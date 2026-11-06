#include "Personagem.h"

#include <utility>

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

int Personagem::getId() const {
    return id_;
}

int Personagem::getIdDono() const {
    return idDono_;
}

const Ficha& Personagem::getFicha() const {
    return ficha_;
}
