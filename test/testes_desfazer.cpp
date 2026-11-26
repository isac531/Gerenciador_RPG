#include <optional>
#include <string>
#include <vector>

#include "../src/business/control/FacadeSingletonController.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/business/log/ILogger.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "TestRunner.h"

// Testes do RF15 / UC17 Desfazer a ultima edicao da ficha, pela fachada
// (Command + Memento, ADR-0008 e ADR-0009).

namespace {

// Repositorio de personagens cuja gravacao de atualizacao pode passar a falhar.
class RepositorioPersonagemInstavel : public RepositorioPersonagemMemoria {
public:
    bool atualizar(const Personagem& personagem) override {
        if (falhar) throw FalhaArmazenamentoException("disco cheio");
        return RepositorioPersonagemMemoria::atualizar(personagem);
    }

    bool falhar = false;
};

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

Ficha fichaNivel(int nivel, int pontosDeVida) {
    return Ficha{"Thorin", nivel, pontosDeVida, Atributos{}};
}

// Fachada com um usuario e o personagem 1 (nivel 3, 27 PV).
struct CenarioDesfazer {
    RepositorioMemoria usuarios;
    RepositorioPersonagemInstavel personagens;
    RepositorioAcessoMemoria acessos;
    LoggerEspiao logger;
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    int dono = 0;
    int id = 0;

    CenarioDesfazer() {
        fachada.inicializar(usuarios, personagens, acessos);
        fachada.definirLogger(logger);
        dono = fachada.criarConta(Usuario("isac", "Senha@123", Papel::MESTRE));
        id = fachada.criarPersonagem(Personagem(dono, fichaNivel(3, 27)));
    }

    ~CenarioDesfazer() { fachada.inicializar(usuarios, personagens, acessos); }

    int nivelAtual() const { return fachada.buscarPersonagem(id)->getFicha().nivel; }
};

}  // namespace

TESTE(fachada_deve_desfazer_a_ultima_edicao_da_ficha) {
    CenarioDesfazer cenario;
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(4, 35));

    const std::optional<Personagem> restaurado = cenario.fachada.desfazerUltimaEdicao();

    VERIFICAR(restaurado.has_value());
    VERIFICAR(restaurado->getId() == cenario.id);
    VERIFICAR(restaurado->getIdDono() == cenario.dono);
    VERIFICAR(restaurado->getFicha().nivel == 3);
    VERIFICAR(restaurado->getFicha().pontosDeVida == 27);
    VERIFICAR(cenario.nivelAtual() == 3);
    VERIFICAR(cenario.logger.registrou("INFO Edicao desfeita: personagem ID 1"));
}

TESTE(fachada_deve_desfazer_apenas_a_ultima_edicao) {
    CenarioDesfazer cenario;
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(4, 35));
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(5, 40));

    VERIFICAR(cenario.fachada.desfazerUltimaEdicao().has_value());
    VERIFICAR(cenario.nivelAtual() == 4);
    VERIFICAR(!cenario.fachada.desfazerUltimaEdicao().has_value());
    VERIFICAR(cenario.nivelAtual() == 4);
}

TESTE(desfazer_sem_edicao_deve_retornar_vazio) {
    CenarioDesfazer cenario;
    VERIFICAR(!cenario.fachada.desfazerUltimaEdicao().has_value());
    VERIFICAR(cenario.nivelAtual() == 3);
}

TESTE(edicao_recusada_nao_deve_substituir_a_ultima_edicao) {
    CenarioDesfazer cenario;
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(4, 35));
    VERIFICAR_LANCA(cenario.fachada.editarPersonagem(cenario.id, Ficha{"", 9, 50, Atributos{}}),
                    FichaInvalidaException);

    cenario.fachada.desfazerUltimaEdicao();
    VERIFICAR(cenario.nivelAtual() == 3);
}

TESTE(desfazer_apos_excluir_o_personagem_nao_deve_recria_lo) {
    CenarioDesfazer cenario;
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(4, 35));
    cenario.fachada.excluirPersonagem(cenario.id);

    VERIFICAR(!cenario.fachada.desfazerUltimaEdicao().has_value());
    VERIFICAR(cenario.fachada.obterQuantidadePersonagens() == 0);
    VERIFICAR(cenario.logger.registrou("WARN Edicao nao desfeita: personagem ID 1 nao existe mais"));
}

TESTE(falha_ao_desfazer_deve_manter_a_edicao_para_nova_tentativa) {
    CenarioDesfazer cenario;
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(4, 35));
    cenario.personagens.falhar = true;

    VERIFICAR_LANCA(cenario.fachada.desfazerUltimaEdicao(), FalhaArmazenamentoException);
    VERIFICAR(cenario.nivelAtual() == 4);
    VERIFICAR(cenario.logger.registrou("ERROR Falha ao desfazer edicao de personagem: disco cheio"));

    cenario.personagens.falhar = false;
    VERIFICAR(cenario.fachada.desfazerUltimaEdicao().has_value());
    VERIFICAR(cenario.nivelAtual() == 3);
}

TESTE(inicializar_deve_esquecer_a_ultima_edicao) {
    CenarioDesfazer cenario;
    cenario.fachada.editarPersonagem(cenario.id, fichaNivel(4, 35));
    cenario.fachada.inicializar(cenario.usuarios, cenario.personagens, cenario.acessos);

    VERIFICAR(!cenario.fachada.desfazerUltimaEdicao().has_value());
    VERIFICAR(cenario.nivelAtual() == 4);
}
