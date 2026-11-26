#include "FacadeSingletonController.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "../command/BuscarPersonagemComando.h"
#include "../command/ContarEntidadesComando.h"
#include "../command/CriarContaComando.h"
#include "../command/CriarPersonagemComando.h"
#include "../command/EditarPersonagemComando.h"
#include "../command/ExcluirPersonagemComando.h"
#include "../command/ExcluirUsuarioComando.h"
#include "../command/FazerLoginComando.h"
#include "../command/GerarRelatorioAcessosComando.h"
#include "../command/ListarPersonagensComando.h"
#include "../command/ListarUsuariosComando.h"

namespace {

template <typename Controle>
Controle& exigirInicializado(const std::unique_ptr<Controle>& controle) {
    if (!controle) {
        throw std::logic_error("FacadeSingletonController usada antes de inicializar().");
    }
    return *controle;
}

}  // namespace

FacadeSingletonController& FacadeSingletonController::getInstancia() {
    // Inicializacao de static local e thread-safe desde o C++11.
    static FacadeSingletonController instancia;
    return instancia;
}

void FacadeSingletonController::inicializar(IUsuarioRepositorio& usuarios,
                                            IPersonagemRepositorio& personagens,
                                            IAcessoRepositorio& acessos,
                                            ControleLogin::Relogio relogio) {
    controleCriarConta_ = std::make_unique<ControleCriarConta>(usuarios);
    controleUsuarios_ = std::make_unique<ControleUsuarios>(usuarios);
    controlePersonagem_ = std::make_unique<ControlePersonagem>(personagens, usuarios);
    controleRelatorio_ = std::make_unique<ControleRelatorioAcessos>(usuarios, acessos, relogio);
    controleLogin_ = std::make_unique<ControleLogin>(usuarios, acessos, std::move(relogio));
    logger_ = nullptr;
    historicoEdicao_.descartar();
}

ControleCriarConta& FacadeSingletonController::controleCriarConta() const {
    return exigirInicializado(controleCriarConta_);
}

ControleUsuarios& FacadeSingletonController::controleUsuarios() const {
    return exigirInicializado(controleUsuarios_);
}

ControlePersonagem& FacadeSingletonController::controlePersonagem() const {
    return exigirInicializado(controlePersonagem_);
}

ControleLogin& FacadeSingletonController::controleLogin() const {
    return exigirInicializado(controleLogin_);
}

ControleRelatorioAcessos& FacadeSingletonController::controleRelatorio() const {
    return exigirInicializado(controleRelatorio_);
}

void FacadeSingletonController::definirLogger(ILogger& logger) {
    logger_ = &logger;
}

ILogger& FacadeSingletonController::logger() const {
    static LoggerNulo nulo;
    return logger_ != nullptr ? *logger_ : nulo;
}

// Em cada metodo abaixo a fachada monta o comando do caso de uso, o invocador
// o executa (e registra falhas de armazenamento) e a fachada devolve o
// resultado guardado no comando.

int FacadeSingletonController::criarConta(const Usuario& usuario) {
    CriarContaComando comando(controleCriarConta(), usuario, logger());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

std::vector<Usuario> FacadeSingletonController::listarUsuarios() const {
    ListarUsuariosComando comando(controleUsuarios());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

bool FacadeSingletonController::excluirUsuario(int id) {
    ExcluirUsuarioComando comando(controlePersonagem(), controleUsuarios(), id, logger());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

Usuario FacadeSingletonController::fazerLogin(const std::string& login,
                                              const std::string& senha) {
    FazerLoginComando comando(controleLogin(), login, senha, logger());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

RelatorioGerado FacadeSingletonController::gerarRelatorioAcessos(FormatoRelatorio formato) const {
    GerarRelatorioAcessosComando comando(controleRelatorio(), formato, logger());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

int FacadeSingletonController::criarPersonagem(const Personagem& personagem) {
    CriarPersonagemComando comando(controlePersonagem(), personagem, logger());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

void FacadeSingletonController::editarPersonagem(int id, const Ficha& ficha) {
    EditarPersonagemComando comando(controlePersonagem(), historicoEdicao_, id, ficha, logger());
    invocador_.executar(comando, logger());
}

bool FacadeSingletonController::excluirPersonagem(int id) {
    ExcluirPersonagemComando comando(controlePersonagem(), id, logger());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

std::optional<Personagem> FacadeSingletonController::buscarPersonagem(int id) const {
    BuscarPersonagemComando comando(controlePersonagem(), id);
    invocador_.executar(comando, logger());
    return comando.resultado();
}

std::vector<Personagem> FacadeSingletonController::listarPersonagens() const {
    ListarPersonagensComando comando(controlePersonagem());
    invocador_.executar(comando, logger());
    return comando.resultado();
}

std::size_t FacadeSingletonController::obterQuantidadeUsuarios() const {
    return contarEntidades().usuarios;
}

std::size_t FacadeSingletonController::obterQuantidadePersonagens() const {
    return contarEntidades().personagens;
}

std::size_t FacadeSingletonController::obterQuantidadeEntidades() const {
    return contarEntidades().total();
}

QuantidadeEntidades FacadeSingletonController::contarEntidades() const {
    ContarEntidadesComando comando(controleUsuarios(), controlePersonagem());
    invocador_.executar(comando, logger());
    return comando.resultado();
}
