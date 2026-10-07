#include "ControleCriarConta.h"

#include <stdexcept>

#include "../validation/RegrasLogin.h"
#include "../validation/RegrasSenha.h"

// Para criar uma nova regra: implemente IRegraValidacao e registre aqui.
// A ordem de registro define a ordem de execucao.
ControleCriarConta::ControleCriarConta() {
    regrasLogin_.push_back(std::make_unique<LoginVazioRegra>());
    regrasLogin_.push_back(std::make_unique<LoginLongoRegra>());
    regrasLogin_.push_back(std::make_unique<LoginComNumeroRegra>());

    regrasSenha_.push_back(std::make_unique<SenhaTamanhoRegra>());
    regrasSenha_.push_back(std::make_unique<SenhaIgualUsuarioRegra>());
    regrasSenha_.push_back(std::make_unique<SenhaCaracteresRegra>());
}

void ControleCriarConta::executar(const Regras& regras, const Usuario& usuario) {
    for (const auto& regra : regras) {
        regra->validar(usuario);
    }
}

void ControleCriarConta::validaLogin(const Usuario& usuario) const {
    executar(regrasLogin_, usuario);
}

void ControleCriarConta::validaSenha(const Usuario& usuario) const {
    executar(regrasSenha_, usuario);
}

void ControleCriarConta::criarConta(const Usuario& usuario) {
    if (usuario.getId() <= 0) {
        throw std::invalid_argument("O ID deve ser um numero positivo.");
    }
    if (usuarios_.count(usuario.getId()) > 0) {
        throw std::invalid_argument("Ja existe um usuario com esse ID.");
    }

    validaLogin(usuario);
    validaSenha(usuario);

    usuarios_.emplace(usuario.getId(), usuario);
}

bool ControleCriarConta::excluirConta(int id) {
    return usuarios_.erase(id) > 0;
}

const std::map<int, Usuario>& ControleCriarConta::listar() const {
    return usuarios_;
}
