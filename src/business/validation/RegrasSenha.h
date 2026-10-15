#pragma once

#include <cstddef>

#include "IRegraValidacao.h"

// Politica de senha padrao do AWS IAM (RF03).
constexpr std::size_t TAMANHO_MINIMO_SENHA = 8;
constexpr std::size_t TAMANHO_MAXIMO_SENHA = 128;
constexpr int MINIMO_TIPOS_CARACTERE_SENHA = 3;

class SenhaTamanhoRegra : public IRegraValidacao {
public:
    SenhaTamanhoRegra(std::size_t minimo = TAMANHO_MINIMO_SENHA,
                      std::size_t maximo = TAMANHO_MAXIMO_SENHA);
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
    explicit SenhaCaracteresRegra(int minimoTipos = MINIMO_TIPOS_CARACTERE_SENHA);
    void validar(const Usuario& usuario) const override;

private:
    int minimoTipos_;
};
