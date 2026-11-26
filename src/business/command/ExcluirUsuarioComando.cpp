#include "ExcluirUsuarioComando.h"

#include <string>

ExcluirUsuarioComando::ExcluirUsuarioComando(ControlePersonagem& personagens,
                                             ControleUsuarios& usuarios, int id, ILogger& logger)
    : personagens_(personagens), usuarios_(usuarios), id_(id), logger_(logger) {}

std::string ExcluirUsuarioComando::descricaoFalha() const {
    return "Falha ao excluir usuario";
}

bool ExcluirUsuarioComando::executarComResultado() {
    // Personagens primeiro: se a exclusao falhar no meio, o usuario continua
    // existindo e nenhum personagem fica sem dono.
    personagens_.excluirDoDono(id_);
    const bool excluido = usuarios_.excluirConta(id_);
    if (excluido) {
        logger_.info("Usuario excluido: ID " + std::to_string(id_));
    }
    return excluido;
}
