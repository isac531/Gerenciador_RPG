#pragma once

#include <utility>

#include "../model/Ficha.h"

class Personagem;

// Memento do padrao Memento (ADR-0009): a ficha de um personagem antes de
// uma edicao. O estado e privado e so o Personagem (Originator) o le; para
// quem guarda o memento (HistoricoEdicaoPersonagem, o Caretaker) ele e
// opaco e so informa de qual personagem e.
class MementoPersonagem {
public:
    int getIdPersonagem() const { return idPersonagem_; }

private:
    friend class Personagem;

    MementoPersonagem(int idPersonagem, Ficha ficha)
        : idPersonagem_(idPersonagem), ficha_(std::move(ficha)) {}

    int idPersonagem_;
    Ficha ficha_;
};
