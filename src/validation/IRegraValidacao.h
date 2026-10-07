#pragma once

#include "../entity/Usuario.h"

// Interface de uma regra de validacao de conta.
// Cada regra concreta lanca a sua propria excecao quando e violada.
class IRegraValidacao {
public:
    virtual ~IRegraValidacao() = default;

    virtual void validar(const Usuario& usuario) const = 0;
};
