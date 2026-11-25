#include "ExcluirPersonagemComando.h"

#include <string>

ExcluirPersonagemComando::ExcluirPersonagemComando(ControlePersonagem& controle, int id,
                                                   ILogger& logger)
    : controle_(controle), id_(id), logger_(logger) {}

std::string ExcluirPersonagemComando::descricaoFalha() const {
    return "Falha ao excluir personagem";
}

bool ExcluirPersonagemComando::executarComResultado() {
    const bool excluido = controle_.excluir(id_);
    if (excluido) {
        logger_.info("Personagem excluido: ID " + std::to_string(id_));
    }
    return excluido;
}
