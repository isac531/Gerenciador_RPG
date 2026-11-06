#pragma once

#include "../model/Ficha.h"

// Interface de uma regra de validacao da ficha do personagem.
// Cada regra concreta lanca a sua propria FichaInvalidaException.
class IRegraFicha {
public:
    virtual ~IRegraFicha() = default;

    virtual void validar(const Ficha& ficha) const = 0;
};
