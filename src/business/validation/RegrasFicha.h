#pragma once

#include <cstddef>

#include "IRegraFicha.h"

// Limites do SRD 5.1 (D&D 5e).
constexpr std::size_t TAMANHO_MAXIMO_NOME_PERSONAGEM = 30;
constexpr int NIVEL_MINIMO = 1;
constexpr int NIVEL_MAXIMO = 20;
constexpr int PONTOS_DE_VIDA_MINIMOS = 1;
constexpr int ATRIBUTO_MINIMO = 1;
constexpr int ATRIBUTO_MAXIMO = 30;

// Nome nao vazio (nem so com espacos) e com no maximo 30 caracteres.
class NomePersonagemRegra : public IRegraFicha {
public:
    void validar(const Ficha& ficha) const override;
};

class NivelRegra : public IRegraFicha {
public:
    void validar(const Ficha& ficha) const override;
};

class PontosDeVidaRegra : public IRegraFicha {
public:
    void validar(const Ficha& ficha) const override;
};

// Cada um dos seis atributos entre 1 e 30.
class AtributosRegra : public IRegraFicha {
public:
    void validar(const Ficha& ficha) const override;
};
