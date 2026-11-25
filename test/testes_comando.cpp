#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../src/business/command/ComandoComResultado.h"
#include "../src/business/command/CriarContaComando.h"
#include "../src/business/command/ExcluirUsuarioComando.h"
#include "../src/business/command/IComando.h"
#include "../src/business/command/InvocadorComandos.h"
#include "../src/business/control/FacadeSingletonController.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/business/log/ILogger.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "TestRunner.h"

namespace {

const std::string kSenhaValida = "Senha@123";

// Logger de teste que guarda as linhas na memoria.
class LoggerEspiao : public ILogger {
public:
    void info(const std::string& mensagem) override { linhas.push_back("INFO " + mensagem); }
    void aviso(const std::string& mensagem) override { linhas.push_back("WARN " + mensagem); }
    void erro(const std::string& mensagem) override { linhas.push_back("ERROR " + mensagem); }

    bool registrou(const std::string& linha) const {
        for (const std::string& registrada : linhas) {
            if (registrada == linha) return true;
        }
        return false;
    }

    std::vector<std::string> linhas;
};

// Comando de teste: roda uma acao qualquer, sem saber nada de usuarios ou personagens.
class ComandoDeTeste : public IComando {
public:
    explicit ComandoDeTeste(std::function<void()> acao) : acao_(std::move(acao)) {}

    void executar() override {
        ++execucoes;
        acao_();
    }
    std::string descricaoFalha() const override { return "Falha ao rodar comando de teste"; }

    int execucoes = 0;

private:
    std::function<void()> acao_;
};

// Repositorio de usuarios cuja gravacao sempre falha (simula disco cheio).
class RepositorioUsuarioSemEspaco : public RepositorioMemoria {
public:
    int adicionar(const Usuario&) override {
        throw FalhaArmazenamentoException("disco cheio");
    }
};

// Controles ligados a repositorios novos em memoria, sem passar pela fachada.
struct CenarioComando {
    RepositorioMemoria usuarios;
    RepositorioPersonagemMemoria personagens;
    ControleCriarConta criarConta{usuarios};
    ControleUsuarios controleUsuarios{usuarios};
    ControlePersonagem controlePersonagem{personagens, usuarios};
    LoggerEspiao logger;
    InvocadorComandos invocador;
};

}  // namespace

// ---------- Command: o pedido vira um objeto ----------

TESTE(comando_nao_deve_executar_ao_ser_criado) {
    CenarioComando cenario;
    CriarContaComando comando(cenario.criarConta, Usuario("isac", kSenhaValida, Papel::JOGADOR),
                              cenario.logger);
    VERIFICAR(cenario.controleUsuarios.quantidade() == 0);
    VERIFICAR_LANCA(comando.resultado(), std::logic_error);
}

TESTE(comando_deve_guardar_o_resultado_depois_de_executado_pelo_invocador) {
    CenarioComando cenario;
    CriarContaComando comando(cenario.criarConta, Usuario("isac", kSenhaValida, Papel::JOGADOR),
                              cenario.logger);
    cenario.invocador.executar(comando, cenario.logger);
    VERIFICAR(comando.resultado() == 1);
    VERIFICAR(cenario.controleUsuarios.quantidade() == 1);
    VERIFICAR(cenario.logger.registrou("INFO Conta criada: ID 1, login isac"));
}

TESTE(comando_de_exclusao_deve_excluir_o_usuario_e_os_personagens_dele) {
    CenarioComando cenario;
    const int dono = cenario.criarConta.criarConta(Usuario("isac", kSenhaValida, Papel::MESTRE));
    cenario.controlePersonagem.criar(Personagem(dono, Ficha{"Thorin", 1, 10, Atributos{}}));

    ExcluirUsuarioComando comando(cenario.controlePersonagem, cenario.controleUsuarios, dono,
                                  cenario.logger);
    cenario.invocador.executar(comando, cenario.logger);

    VERIFICAR(comando.resultado());
    VERIFICAR(cenario.controleUsuarios.quantidade() == 0);
    VERIFICAR(cenario.controlePersonagem.quantidade() == 0);
}

TESTE(comando_com_excecao_de_negocio_deve_propagar_e_nao_ter_resultado) {
    CenarioComando cenario;
    CriarContaComando comando(cenario.criarConta, Usuario("isac", "fraca", Papel::JOGADOR),
                              cenario.logger);
    VERIFICAR_LANCA(cenario.invocador.executar(comando, cenario.logger), SenhaInvalidaException);
    VERIFICAR_LANCA(comando.resultado(), std::logic_error);
    VERIFICAR(cenario.controleUsuarios.quantidade() == 0);
}

// ---------- Invoker: executa qualquer comando e trata a falha de armazenamento ----------

TESTE(invocador_deve_executar_qualquer_comando) {
    CenarioComando cenario;
    ComandoDeTeste comando([] {});
    cenario.invocador.executar(comando, cenario.logger);
    VERIFICAR(comando.execucoes == 1);
    VERIFICAR(cenario.logger.linhas.empty());
}

TESTE(invocador_deve_registrar_falha_de_armazenamento_e_relancar) {
    CenarioComando cenario;
    ComandoDeTeste comando([] { throw FalhaArmazenamentoException("disco cheio"); });
    VERIFICAR_LANCA(cenario.invocador.executar(comando, cenario.logger),
                    FalhaArmazenamentoException);
    VERIFICAR(cenario.logger.registrou("ERROR Falha ao rodar comando de teste: disco cheio"));
}

TESTE(invocador_nao_deve_registrar_como_falha_as_excecoes_de_negocio) {
    CenarioComando cenario;
    ComandoDeTeste comando([] { throw IdInvalidoException("Usuario inexistente."); });
    VERIFICAR_LANCA(cenario.invocador.executar(comando, cenario.logger), IdInvalidoException);
    VERIFICAR(cenario.logger.linhas.empty());
}

// ---------- Facade: usa o invocador em todos os casos de uso ----------

TESTE(fachada_deve_registrar_no_log_a_falha_de_armazenamento_ao_criar_conta) {
    RepositorioUsuarioSemEspaco usuarios;
    RepositorioPersonagemMemoria personagens;
    RepositorioAcessoMemoria acessos;
    LoggerEspiao logger;
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    fachada.inicializar(usuarios, personagens, acessos);
    fachada.definirLogger(logger);

    VERIFICAR_LANCA(fachada.criarConta(Usuario("isac", kSenhaValida, Papel::JOGADOR)),
                    FalhaArmazenamentoException);
    VERIFICAR(logger.registrou("ERROR Falha ao criar conta: disco cheio"));

    fachada.inicializar(usuarios, personagens, acessos);  // volta ao logger nulo
}
