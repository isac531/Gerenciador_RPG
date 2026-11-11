#include "FacadeSingletonController.h"

#include "../exception/Excecoes.h"

#include <stdexcept>
#include <string>
#include <utility>

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

RelatorioGerado FacadeSingletonController::gerarRelatorioAcessos(FormatoRelatorio formato) const {
    RelatorioGerado relatorio = controleRelatorio().gerar(formato);
    logger().info("Relatorio de acessos gerado em " + relatorio.extensao);
    return relatorio;
}

void FacadeSingletonController::definirLogger(ILogger& logger) {
    logger_ = &logger;
}

ILogger& FacadeSingletonController::logger() const {
    static LoggerNulo nulo;
    return logger_ != nullptr ? *logger_ : nulo;
}

template <typename Operacao>
auto FacadeSingletonController::registrandoFalhaDeArmazenamento(const std::string& acao,
                                                                Operacao operacao)
    -> decltype(operacao()) {
    try {
        return operacao();
    } catch (const FalhaArmazenamentoException& e) {
        logger().erro(acao + ": " + e.what());
        throw;
    }
}

Usuario FacadeSingletonController::fazerLogin(const std::string& login,
                                              const std::string& senha) {
    try {
        Usuario usuario = registrandoFalhaDeArmazenamento(
            "Falha ao registrar acesso", [&] { return controleLogin().autenticar(login, senha); });
        logger().info("Login aceito: " + login);
        return usuario;
    } catch (const CredenciaisInvalidasException&) {
        logger().aviso("Login recusado: " + login);
        throw;
    }
}

int FacadeSingletonController::criarConta(const Usuario& usuario) {
    // A senha nunca vai para o log (NF-SEG-01); as mensagens das excecoes
    // de senha tambem nao a repetem.
    try {
        const int id = registrandoFalhaDeArmazenamento(
            "Falha ao criar conta", [&] { return controleCriarConta().criarConta(usuario); });
        logger().info("Conta criada: ID " + std::to_string(id) + ", login " + usuario.getLogin());
        return id;
    } catch (const LoginInvalidoException& e) {
        logger().aviso("Cadastro recusado (" + usuario.getLogin() + "): " + e.what());
        throw;
    } catch (const SenhaInvalidaException& e) {
        logger().aviso("Cadastro recusado (" + usuario.getLogin() + "): " + e.what());
        throw;
    }
}

std::vector<Usuario> FacadeSingletonController::listarUsuarios() const {
    return controleUsuarios().listar();
}

bool FacadeSingletonController::excluirUsuario(int id) {
    // Personagens primeiro: se a exclusao falhar no meio, o usuario continua
    // existindo e nenhum personagem fica sem dono.
    return registrandoFalhaDeArmazenamento("Falha ao excluir usuario", [&] {
        controlePersonagem().excluirDoDono(id);
        const bool excluido = controleUsuarios().excluirConta(id);
        if (excluido) {
            logger().info("Usuario excluido: ID " + std::to_string(id));
        }
        return excluido;
    });
}

int FacadeSingletonController::criarPersonagem(const Personagem& personagem) {
    const int id = registrandoFalhaDeArmazenamento(
        "Falha ao criar personagem", [&] { return controlePersonagem().criar(personagem); });
    logger().info("Personagem criado: ID " + std::to_string(id) + ", dono " +
                  std::to_string(personagem.getIdDono()));
    return id;
}

void FacadeSingletonController::editarPersonagem(int id, const Ficha& ficha) {
    registrandoFalhaDeArmazenamento("Falha ao editar personagem",
                                    [&] { controlePersonagem().editarFicha(id, ficha); });
    logger().info("Ficha editada: personagem ID " + std::to_string(id));
}

bool FacadeSingletonController::excluirPersonagem(int id) {
    const bool excluido = registrandoFalhaDeArmazenamento(
        "Falha ao excluir personagem", [&] { return controlePersonagem().excluir(id); });
    if (excluido) {
        logger().info("Personagem excluido: ID " + std::to_string(id));
    }
    return excluido;
}

std::optional<Personagem> FacadeSingletonController::buscarPersonagem(int id) const {
    return controlePersonagem().buscar(id);
}

std::vector<Personagem> FacadeSingletonController::listarPersonagens() const {
    return controlePersonagem().listar();
}

std::size_t FacadeSingletonController::obterQuantidadeUsuarios() const {
    return controleUsuarios().quantidade();
}

std::size_t FacadeSingletonController::obterQuantidadePersonagens() const {
    return controlePersonagem().quantidade();
}

std::size_t FacadeSingletonController::obterQuantidadeEntidades() const {
    return obterQuantidadeUsuarios() + obterQuantidadePersonagens();
}
