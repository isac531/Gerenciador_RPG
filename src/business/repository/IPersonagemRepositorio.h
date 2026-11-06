#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "../model/Personagem.h"

// Contrato de armazenamento de personagens, no mesmo molde de
// IUsuarioRepositorio. Implementacoes em src/infra.
class IPersonagemRepositorio {
public:
    virtual ~IPersonagemRepositorio() = default;

    // Atribui o proximo ID livre (nunca reaproveitado), guarda e retorna o ID.
    virtual int adicionar(const Personagem& personagem) = 0;

    // Substitui o personagem de mesmo ID. Retorna false se o ID nao existir.
    virtual bool atualizar(const Personagem& personagem) = 0;

    // Retorna false se o ID nao existir.
    virtual bool remover(int id) = 0;

    virtual std::optional<Personagem> buscar(int id) const = 0;

    // Personagens em ordem crescente de ID.
    virtual std::vector<Personagem> listarTodos() const = 0;

    virtual std::size_t quantidade() const = 0;
};
