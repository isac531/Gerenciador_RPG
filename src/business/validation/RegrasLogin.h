#pragma once

#include <cstddef>

#include "../repository/IUsuarioRepositorio.h"
#include "IRegraValidacao.h"

constexpr std::size_t TAMANHO_MAXIMO_LOGIN = 12;

class LoginVazioRegra : public IRegraValidacao {
public:
    void validar(const Usuario& usuario) const override;
};

class LoginLongoRegra : public IRegraValidacao {
public:
    explicit LoginLongoRegra(std::size_t maximo = TAMANHO_MAXIMO_LOGIN);
    void validar(const Usuario& usuario) const override;

private:
    std::size_t maximo_;
};

class LoginComNumeroRegra : public IRegraValidacao {
public:
    void validar(const Usuario& usuario) const override;
};

// Recusa um login ja cadastrado (comparacao sensivel a maiusculas).
class LoginUnicoRegra : public IRegraValidacao {
public:
    explicit LoginUnicoRegra(const IUsuarioRepositorio& repositorio);
    void validar(const Usuario& usuario) const override;

private:
    const IUsuarioRepositorio& repositorio_;
};
