#pragma once

#include <string>

#include "Papel.h"

// Valor do ID de um usuario que ainda nao foi guardado.
constexpr int SEM_ID = 0;

class Usuario {
public:
    Usuario() = default;

    // Usuario ainda sem ID: o repositorio atribui o ID ao guardar (ver ADR-0003).
    Usuario(std::string login, std::string senha, Papel papel);

    Usuario(int id, std::string login, std::string senha, Papel papel);

    // Copia deste usuario com o ID informado.
    Usuario comId(int id) const;

    int getId() const;
    const std::string& getLogin() const;
    const std::string& getSenha() const;
    Papel getPapel() const;

    // Retorna true se login e senha informados correspondem a este usuario.
    bool fazerLogin(const std::string& login, const std::string& senha) const;

private:
    int id_ = SEM_ID;
    std::string login_;
    std::string senha_;
    Papel papel_ = Papel::JOGADOR;
};
