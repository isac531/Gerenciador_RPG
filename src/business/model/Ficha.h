#pragma once

#include <string>

// Seis atributos de um personagem (glossario: Atributo), no padrao SRD 5.1.
struct Atributos {
    int forca = 10;
    int destreza = 10;
    int constituicao = 10;
    int inteligencia = 10;
    int sabedoria = 10;
    int carisma = 10;
};

// Dados de jogo de um personagem (glossario: Ficha).
struct Ficha {
    std::string nome;
    int nivel = 1;
    int pontosDeVida = 1;
    Atributos atributos;
};

// Modificador de um atributo: (valor - 10) / 2, arredondado para baixo.
// Ex.: 16 -> +3; 9 -> -1.
inline int calcularModificador(int valorAtributo) {
    const int diferenca = valorAtributo - 10;
    return diferenca >= 0 ? diferenca / 2 : (diferenca - 1) / 2;
}
