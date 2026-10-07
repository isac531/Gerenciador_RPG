#pragma once

#include <cstddef>

#include "IRegraValidacao.h"

class LoginVazioRegra : public IRegraValidacao {
public:
    void validar(const Usuario& usuario) const override;
};

class LoginLongoRegra : public IRegraValidacao {
public:
    explicit LoginLongoRegra(std::size_t maximo = 12);
    void validar(const Usuario& usuario) const override;

private:
    std::size_t maximo_;
};

class LoginComNumeroRegra : public IRegraValidacao {
public:
    void validar(const Usuario& usuario) const override;
};
