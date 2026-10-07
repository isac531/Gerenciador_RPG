#pragma once

#include <cstddef>

#include "IRegraValidacao.h"

class SenhaTamanhoRegra : public IRegraValidacao {
public:
    SenhaTamanhoRegra(std::size_t minimo = 8, std::size_t maximo = 128);
    void validar(const Usuario& usuario) const override;

private:
    std::size_t minimo_;
    std::size_t maximo_;
};

class SenhaIgualUsuarioRegra : public IRegraValidacao {
public:
    void validar(const Usuario& usuario) const override;
};

// Exige ao menos `minimoTipos` dos 4 tipos de caractere:
// maiusculas, minusculas, numeros e especiais ! @ # $ % ^ & * ( ) _ + - = [ ] { } | '
class SenhaCaracteresRegra : public IRegraValidacao {
public:
    explicit SenhaCaracteresRegra(int minimoTipos = 3);
    void validar(const Usuario& usuario) const override;

private:
    int minimoTipos_;
};
