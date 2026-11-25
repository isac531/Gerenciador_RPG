#include "EditarPersonagemComando.h"

#include <string>
#include <utility>

EditarPersonagemComando::EditarPersonagemComando(ControlePersonagem& controle, int id,
                                                 Ficha ficha, ILogger& logger)
    : controle_(controle), id_(id), ficha_(std::move(ficha)), logger_(logger) {}

void EditarPersonagemComando::executar() {
    controle_.editarFicha(id_, ficha_);
    logger_.info("Ficha editada: personagem ID " + std::to_string(id_));
}

std::string EditarPersonagemComando::descricaoFalha() const {
    return "Falha ao editar personagem";
}
