#pragma once

#include <cstddef>
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

    // Atribui ao usuario o proximo ID livre, guarda-o e retorna esse ID.
    // O ID que vier no objeto recebido e ignorado. IDs nunca sao
    // reaproveitados, mesmo depois de uma exclusao (ver ADR-0003).
    virtual int adicionar(const Usuario& usuario) = 0;

    // Retorna false se o ID nao existir.
    virtual bool remover(int id) = 0;

    virtual bool existe(int id) const = 0;
    virtual std::optional<Usuario> buscar(int id) const = 0;

    // Usuarios em ordem crescente de ID.
    virtual std::vector<Usuario> listarTodos() const = 0;

    virtual std::size_t quantidade() const = 0;
};
