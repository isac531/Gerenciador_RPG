#include "RegrasSenha.h"

#include <algorithm>
#include <cctype>

#include "../exception/Excecoes.h"

SenhaTamanhoRegra::SenhaTamanhoRegra(std::size_t minimo, std::size_t maximo)
    : minimo_(minimo), maximo_(maximo) {}

void SenhaTamanhoRegra::validar(const Usuario& usuario) const {
    std::size_t tamanho = usuario.getSenha().size();
    if (tamanho < minimo_ || tamanho > maximo_) {
        throw SenhaTamanhoInvalidoException();
    }
}

void SenhaIgualUsuarioRegra::validar(const Usuario& usuario) const {
    if (usuario.getSenha() == usuario.getNome()) {
        throw SenhaIgualUsuarioException();
    }
}

SenhaCaracteresRegra::SenhaCaracteresRegra(int minimoTipos)
    : minimoTipos_(minimoTipos) {}

void SenhaCaracteresRegra::validar(const Usuario& usuario) const {
    static const std::string kEspeciais = "!@#$%^&*()_+-=[]{}|'";
    const std::string& senha = usuario.getSenha();

    auto possuiTipo = [&senha](auto predicado) {
        return std::any_of(senha.begin(), senha.end(), predicado);
    };

    const int tipos =
        possuiTipo([](unsigned char c) { return std::isupper(c) != 0; }) +
        possuiTipo([](unsigned char c) { return std::islower(c) != 0; }) +
        possuiTipo([](unsigned char c) { return std::isdigit(c) != 0; }) +
        possuiTipo([](unsigned char c) { return kEspeciais.find(static_cast<char>(c)) != std::string::npos; });

    if (tipos < minimoTipos_) {
        throw SenhaCaracteresInsuficientesException();
    }
}
