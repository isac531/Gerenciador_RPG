#pragma once

#include <optional>
#include <vector>

#include "../model/Usuario.h"

// Contrato de armazenamento de usuarios.
// A camada de negocio depende apenas desta interface; as implementacoes
// concretas (memoria, arquivo binario) ficam em src/infra.
// Implementacoes que acessam disco lancam FalhaArmazenamentoException.
class IUsuarioRepositorio {
public:
    virtual ~IUsuarioRepositorio() = default;

    // Retorna false se ja existir um usuario com o mesmo ID.
    virtual bool adicionar(const Usuario& usuario) = 0;

    // Retorna false se o ID nao existir.
    virtual bool remover(int id) = 0;

    virtual bool existe(int id) const = 0;
    virtual std::optional<Usuario> buscar(int id) const = 0;

    // Usuarios em ordem crescente de ID.
    virtual std::vector<Usuario> listarTodos() const = 0;
};
