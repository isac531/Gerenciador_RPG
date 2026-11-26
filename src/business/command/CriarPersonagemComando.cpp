#include "CriarPersonagemComando.h"

#include <string>
#include <utility>

CriarPersonagemComando::CriarPersonagemComando(ControlePersonagem& controle,
                                               Personagem personagem, ILogger& logger)
    : controle_(controle), personagem_(std::move(personagem)), logger_(logger) {}

std::string CriarPersonagemComando::descricaoFalha() const {
    return "Falha ao criar personagem";
}

int CriarPersonagemComando::executarComResultado() {
    const int id = controle_.criar(personagem_);
    logger_.info("Personagem criado: ID " + std::to_string(id) + ", dono " +
                 std::to_string(personagem_.getIdDono()));
    return id;
}
