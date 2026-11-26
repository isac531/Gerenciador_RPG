#include "BuscarPersonagemComando.h"

BuscarPersonagemComando::BuscarPersonagemComando(const ControlePersonagem& controle, int id)
    : controle_(controle), id_(id) {}

std::string BuscarPersonagemComando::descricaoFalha() const {
    return "Falha ao buscar personagem";
}

std::optional<Personagem> BuscarPersonagemComando::executarComResultado() {
    return controle_.buscar(id_);
}
