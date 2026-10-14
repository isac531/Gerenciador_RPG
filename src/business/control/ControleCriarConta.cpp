#include "ControleCriarConta.h"


#include "../validation/RegrasLogin.h"
#include "../validation/RegrasSenha.h"

// Para criar uma nova regra: implemente IRegraValidacao e registre aqui.
// A ordem de registro define a ordem de execucao.
ControleCriarConta::ControleCriarConta(IUsuarioRepositorio& repositorio)
    : repositorio_(repositorio) {

    regrasLogin_.push_back(std::make_unique<LoginVazioRegra>());
    regrasLogin_.push_back(std::make_unique<LoginLongoRegra>());
    regrasLogin_.push_back(std::make_unique<LoginComNumeroRegra>());
    regrasLogin_.push_back(std::make_unique<LoginUnicoRegra>(repositorio_));

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

void ControleCriarConta::validaId(const Usuario& usuario) const {
    if (usuario.getId() <= 0) {
        throw IdInvalidoException("O ID deve ser um numero positivo.");
    }
    if (repositorio_.existe(usuario.getId())) {
        throw IdInvalidoException("Ja existe um usuario com esse ID.");
    }
}

void ControleCriarConta::criarConta(const Usuario& usuario) {
    validaId(usuario);
    validaLogin(usuario);
    validaSenha(usuario);

    if (!repositorio_.adicionar(usuario)) {
        throw IdInvalidoException("Ja existe um usuario com esse ID.");
    }
}
