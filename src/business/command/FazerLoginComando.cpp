#include "FazerLoginComando.h"

#include <utility>

#include "../exception/Excecoes.h"

FazerLoginComando::FazerLoginComando(ControleLogin& controle, std::string login,
                                     std::string senha, ILogger& logger)
    : controle_(controle), login_(std::move(login)), senha_(std::move(senha)), logger_(logger) {}

std::string FazerLoginComando::descricaoFalha() const {
    return "Falha ao registrar acesso";
}

Usuario FazerLoginComando::executarComResultado() {
    // Nem a senha informada nem a cadastrada vao para o log (NF-SEG-01).
    try {
        Usuario usuario = controle_.autenticar(login_, senha_);
        logger_.info("Login aceito: " + login_);
        return usuario;
    } catch (const CredenciaisInvalidasException&) {
        logger_.aviso("Login recusado: " + login_);
        throw;
    }
}
