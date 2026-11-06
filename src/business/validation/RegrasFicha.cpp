#include "RegrasFicha.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "../exception/Excecoes.h"

namespace {

bool estaEntre(int valor, int minimo, int maximo) {
    return valor >= minimo && valor <= maximo;
}

void validarAtributo(const char* nome, int valor) {
    if (!estaEntre(valor, ATRIBUTO_MINIMO, ATRIBUTO_MAXIMO)) {
        throw AtributoInvalidoException(nome);
    }
}

}  // namespace

void NomePersonagemRegra::validar(const Ficha& ficha) const {
    const std::string& nome = ficha.nome;
    const bool vazio = std::all_of(nome.begin(), nome.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    });
    if (vazio || nome.size() > TAMANHO_MAXIMO_NOME_PERSONAGEM) {
        throw NomePersonagemInvalidoException();
    }
}

void NivelRegra::validar(const Ficha& ficha) const {
    if (!estaEntre(ficha.nivel, NIVEL_MINIMO, NIVEL_MAXIMO)) {
        throw NivelInvalidoException();
    }
}

void PontosDeVidaRegra::validar(const Ficha& ficha) const {
    if (ficha.pontosDeVida < PONTOS_DE_VIDA_MINIMOS) {
        throw PontosDeVidaInvalidosException();
    }
}

void AtributosRegra::validar(const Ficha& ficha) const {
    const Atributos& atributos = ficha.atributos;
    validarAtributo("Forca", atributos.forca);
    validarAtributo("Destreza", atributos.destreza);
    validarAtributo("Constituicao", atributos.constituicao);
    validarAtributo("Inteligencia", atributos.inteligencia);
    validarAtributo("Sabedoria", atributos.sabedoria);
    validarAtributo("Carisma", atributos.carisma);
}
