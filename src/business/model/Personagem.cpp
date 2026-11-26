#include "Personagem.h"

#include <utility>

#include "../exception/Excecoes.h"

Personagem::Personagem(int idDono, Ficha ficha)
    : Personagem(SEM_ID, idDono, std::move(ficha)) {}

Personagem::Personagem(int id, int idDono, Ficha ficha)
    : id_(id), idDono_(idDono), ficha_(std::move(ficha)) {}

Personagem Personagem::comId(int id) const {
    return Personagem(id, idDono_, ficha_);
}

Personagem Personagem::comFicha(Ficha ficha) const {
    return Personagem(id_, idDono_, std::move(ficha));
}

MementoPersonagem Personagem::criarMemento() const {
    return MementoPersonagem(id_, ficha_);
}

Personagem Personagem::restaurar(const MementoPersonagem& memento) const {
    if (memento.idPersonagem_ != id_) {
        throw IdInvalidoException("O memento e de outro personagem.");
    }
    return comFicha(memento.ficha_);
}

int Personagem::getId() const {
    return id_;
}

int Personagem::getIdDono() const {
    return idDono_;
}

const Ficha& Personagem::getFicha() const {
    return ficha_;
}
