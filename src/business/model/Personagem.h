#pragma once

#include "../memento/MementoPersonagem.h"
#include "Ficha.h"
#include "Usuario.h"

// Personagem de um usuario (Usuario 1 -> * Personagem).
// Imutavel: editar a ficha gera uma copia (comFicha).
// E o Originator do padrao Memento (ADR-0009): cria o memento com a ficha
// atual e se restaura a partir dele.
class Personagem {
public:
    Personagem() = default;

    // Personagem ainda sem ID: o repositorio atribui o ID ao guardar.
    Personagem(int idDono, Ficha ficha);

    Personagem(int id, int idDono, Ficha ficha);

    Personagem comId(int id) const;
    Personagem comFicha(Ficha ficha) const;

    // Guarda a ficha atual, para desfazer uma edicao depois.
    MementoPersonagem criarMemento() const;

    // Copia deste personagem com a ficha guardada no memento; ID e dono
    // nao mudam. Lanca IdInvalidoException se o memento for de outro
    // personagem.
    Personagem restaurar(const MementoPersonagem& memento) const;

    int getId() const;
    int getIdDono() const;
    const Ficha& getFicha() const;

private:
    int id_ = SEM_ID;
    int idDono_ = SEM_ID;
    Ficha ficha_;
};
