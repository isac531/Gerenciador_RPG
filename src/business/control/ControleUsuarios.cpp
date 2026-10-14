#include "ControleUsuarios.h"

ControleUsuarios::ControleUsuarios(IUsuarioRepositorio& repositorio)
    : repositorio_(repositorio) {}

std::vector<Usuario> ControleUsuarios::listar() const {
    return repositorio_.listarTodos();
}

bool ControleUsuarios::excluirConta(int id) {
    return repositorio_.remover(id);
}
