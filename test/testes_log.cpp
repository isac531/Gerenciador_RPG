#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../src/business/control/FacadeSingletonController.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/business/log/ILogger.h"
#include "../src/infra/LogCAdapter.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "ArquivoTemporario.h"
#include "TestRunner.h"

namespace {

std::string lerArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    std::stringstream conteudo;
    conteudo << arquivo.rdbuf();
    return conteudo.str();
}

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}

// Logger de teste que guarda as linhas na memoria.
class LoggerEmMemoria : public ILogger {
public:
    void info(const std::string& mensagem) override { linhas.push_back("INFO " + mensagem); }
    void aviso(const std::string& mensagem) override { linhas.push_back("WARN " + mensagem); }
    void erro(const std::string& mensagem) override { linhas.push_back("ERROR " + mensagem); }

    std::string tudo() const {
        std::string texto;
        for (const auto& linha : linhas) texto += linha + '\n';
        return texto;
    }

    std::vector<std::string> linhas;
};

}  // namespace

// ---------- Adapter: LogCAdapter adapta a biblioteca log.c a ILogger ----------

TESTE(log_adapter_deve_gravar_nivel_e_mensagem_no_arquivo) {
    ArquivoTemporario arquivo("teste_rpg.log");
    {
        LogCAdapter logger(arquivo.caminho());
        VERIFICAR(logger.ativo());
        logger.info("Sistema iniciado");
        logger.aviso("Login recusado: bia");
        logger.erro("Falha ao gravar");
    }
    const std::string conteudo = lerArquivo(arquivo.caminho());
    VERIFICAR(contem(conteudo, "INFO  Sistema iniciado"));
    VERIFICAR(contem(conteudo, "WARN  Login recusado: bia"));
    VERIFICAR(contem(conteudo, "ERROR Falha ao gravar"));
}

TESTE(log_adapter_nao_deve_interpretar_porcentagem_como_formato) {
    ArquivoTemporario arquivo("teste_rpg_formato.log");
    {
        LogCAdapter logger(arquivo.caminho());
        logger.aviso("Login recusado: %s%n");
    }
    VERIFICAR(contem(lerArquivo(arquivo.caminho()), "Login recusado: %s%n"));
}

TESTE(log_adapter_com_caminho_invalido_deve_ficar_desligado) {
    LogCAdapter logger("/pasta/que/nao/existe/rpg.log");
    VERIFICAR(!logger.ativo());
    logger.info("nao deve quebrar");
}

// ---------- A fachada usa so a interface ILogger ----------

TESTE(fachada_deve_registrar_login_aceito_e_recusado_sem_a_senha) {
    RepositorioMemoria usuarios;
    RepositorioPersonagemMemoria personagens;
    RepositorioAcessoMemoria acessos;
    LoggerEmMemoria logger;
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    fachada.inicializar(usuarios, personagens, acessos);
    fachada.definirLogger(logger);

    fachada.criarConta(Usuario("isac", "Dado#2026", Papel::MESTRE));
    fachada.fazerLogin("isac", "Dado#2026");
    VERIFICAR_LANCA(fachada.fazerLogin("isac", "Errada#99"), CredenciaisInvalidasException);
    VERIFICAR_LANCA(fachada.criarConta(Usuario("bia", "fraca", Papel::JOGADOR)),
                    SenhaInvalidaException);

    const std::string log = logger.tudo();
    VERIFICAR(contem(log, "INFO Conta criada: ID 1, login isac"));
    VERIFICAR(contem(log, "INFO Login aceito: isac"));
    VERIFICAR(contem(log, "WARN Login recusado: isac"));
    VERIFICAR(contem(log, "WARN Cadastro recusado (bia)"));
    VERIFICAR(!contem(log, "Dado#2026"));
    VERIFICAR(!contem(log, "Errada#99"));
    VERIFICAR(!contem(log, "fraca"));

    fachada.inicializar(usuarios, personagens, acessos);  // volta ao logger nulo
}
