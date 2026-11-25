#include "ListarUsuariosComando.h"

ListarUsuariosComando::ListarUsuariosComando(const ControleUsuarios& controle)
    : controle_(controle) {}

std::string ListarUsuariosComando::descricaoFalha() const {
    return "Falha ao listar usuarios";
}

std::vector<Usuario> ListarUsuariosComando::executarComResultado() {
    return controle_.listar();
}
