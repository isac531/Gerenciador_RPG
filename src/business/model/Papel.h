#pragma once

#include <cstdint>

// Tipo de usuario no sistema de RPG.
// Os valores numericos sao gravados no arquivo binario; nao os altere.
enum class Papel : std::uint8_t {
    MESTRE = 1,
    JOGADOR = 2
};

inline const char* papelParaTexto(Papel papel) {
    switch (papel) {
    case Papel::MESTRE:
        return "Mestre";
    case Papel::JOGADOR:
        return "Jogador";
    }
    return "Desconhecido";
}

// Converte o codigo numerico (1 = Mestre, 2 = Jogador).
// Retorna false se o codigo nao corresponder a nenhum papel.
inline bool papelDeCodigo(int codigo, Papel& papel) {
    switch (codigo) {
    case static_cast<int>(Papel::MESTRE):
        papel = Papel::MESTRE;
        return true;
    case static_cast<int>(Papel::JOGADOR):
        papel = Papel::JOGADOR;
        return true;
    default:
        return false;
    }
}
