#pragma once

#include <vector>

#include "../model/Usuario.h"
#include "../repository/IUsuarioRepositorio.h"

// Controle dos casos de uso UC03 Listar usuarios e UC04 Excluir conta.
class ControleUsuarios {
public:
    explicit ControleUsuarios(IUsuarioRepositorio& repositorio);

    // Usuarios cadastrados em ordem crescente de ID.
    std::vector<Usuario> listar() const;

    // Retorna false se o ID nao existir. Pode lancar FalhaArmazenamentoException.
    bool excluirConta(int id);

private:
    IUsuarioRepositorio& repositorio_;
};
