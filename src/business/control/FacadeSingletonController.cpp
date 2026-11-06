#include "FacadeSingletonController.h"

#include <stdexcept>

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
                                            IPersonagemRepositorio& personagens) {
    controleCriarConta_ = std::make_unique<ControleCriarConta>(usuarios);
    controleUsuarios_ = std::make_unique<ControleUsuarios>(usuarios);
    controlePersonagem_ = std::make_unique<ControlePersonagem>(personagens, usuarios);
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

int FacadeSingletonController::criarConta(const Usuario& usuario) {
    return controleCriarConta().criarConta(usuario);
}

std::vector<Usuario> FacadeSingletonController::listarUsuarios() const {
    return controleUsuarios().listar();
}

bool FacadeSingletonController::excluirUsuario(int id) {
    // Personagens primeiro: se a exclusao falhar no meio, o usuario continua
    // existindo e nenhum personagem fica sem dono.
    controlePersonagem().excluirDoDono(id);
    return controleUsuarios().excluirConta(id);
}

int FacadeSingletonController::criarPersonagem(const Personagem& personagem) {
    return controlePersonagem().criar(personagem);
}

void FacadeSingletonController::editarPersonagem(int id, const Ficha& ficha) {
    controlePersonagem().editarFicha(id, ficha);
}

bool FacadeSingletonController::excluirPersonagem(int id) {
    return controlePersonagem().excluir(id);
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
