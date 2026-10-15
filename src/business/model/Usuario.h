#pragma once

#include <string>

#include "Papel.h"

class Usuario {
public:
    Usuario() = default;
    Usuario(int id, std::string login, std::string senha, Papel papel);

    int getId() const;
    const std::string& getLogin() const;
    const std::string& getSenha() const;
    Papel getPapel() const;

    // Retorna true se login e senha informados correspondem a este usuario.
    bool fazerLogin(const std::string& login, const std::string& senha) const;

private:
    int id_ = 0;
    std::string login_;
    std::string senha_;
    Papel papel_ = Papel::JOGADOR;
};
