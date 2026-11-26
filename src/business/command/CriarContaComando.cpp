#include "CriarContaComando.h"

#include <string>
#include <utility>

#include "../exception/Excecoes.h"

CriarContaComando::CriarContaComando(ControleCriarConta& controle, Usuario usuario,
                                     ILogger& logger)
    : controle_(controle), usuario_(std::move(usuario)), logger_(logger) {}

std::string CriarContaComando::descricaoFalha() const {
    return "Falha ao criar conta";
}

int CriarContaComando::executarComResultado() {
    // A senha nunca vai para o log (NF-SEG-01); as mensagens das excecoes
    // de senha tambem nao a repetem.
    try {
        const int id = controle_.criarConta(usuario_);
        logger_.info("Conta criada: ID " + std::to_string(id) + ", login " + usuario_.getLogin());
        return id;
    } catch (const LoginInvalidoException& e) {
        logger_.aviso("Cadastro recusado (" + usuario_.getLogin() + "): " + e.what());
        throw;
    } catch (const SenhaInvalidaException& e) {
        logger_.aviso("Cadastro recusado (" + usuario_.getLogin() + "): " + e.what());
        throw;
    }
}
