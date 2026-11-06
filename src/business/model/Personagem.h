#pragma once

#include "Ficha.h"
#include "Usuario.h"

// Personagem de um usuario (Usuario 1 -> * Personagem).
// Imutavel: editar a ficha gera uma copia (comFicha).
class Personagem {
public:
    Personagem() = default;

    // Personagem ainda sem ID: o repositorio atribui o ID ao guardar.
    Personagem(int idDono, Ficha ficha);

    Personagem(int id, int idDono, Ficha ficha);

    Personagem comId(int id) const;
    Personagem comFicha(Ficha ficha) const;

    int getId() const;
    int getIdDono() const;
    const Ficha& getFicha() const;

private:
    int id_ = SEM_ID;
    int idDono_ = SEM_ID;
    Ficha ficha_;
};
