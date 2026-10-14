#include "RegrasLogin.h"

#include <algorithm>
#include <cctype>

#include "../exception/Excecoes.h"

void LoginVazioRegra::validar(const Usuario& usuario) const {
    const std::string& login = usuario.getNome();
    bool vazio = std::all_of(login.begin(), login.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    });
    if (vazio) {
        throw LoginVazioException();
    }
}

LoginLongoRegra::LoginLongoRegra(std::size_t maximo) : maximo_(maximo) {}

void LoginLongoRegra::validar(const Usuario& usuario) const {
    if (usuario.getNome().size() > maximo_) {
        throw LoginLongoException();
    }
}

void LoginComNumeroRegra::validar(const Usuario& usuario) const {
    const std::string& login = usuario.getNome();
    bool temNumero = std::any_of(login.begin(), login.end(), [](unsigned char c) {
        return std::isdigit(c) != 0;
    });
    if (temNumero) {
        throw LoginComNumeroException();
    }
}
