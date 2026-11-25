#include "ListarPersonagensComando.h"

ListarPersonagensComando::ListarPersonagensComando(const ControlePersonagem& controle)
    : controle_(controle) {}

std::string ListarPersonagensComando::descricaoFalha() const {
    return "Falha ao listar personagens";
}

std::vector<Personagem> ListarPersonagensComando::executarComResultado() {
    return controle_.listar();
}
