#pragma once

#include <string>

class Usuario {
public:
    Usuario() = default;
    Usuario(int id, std::string login, std::string senha);

    int getId() const;
    const std::string& getLogin() const;
    const std::string& getSenha() const;

    // Retorna true se login e senha informados correspondem a este usuario.
    bool fazerLogin(const std::string& login, const std::string& senha) const;

private:
    int id_ = 0;
    std::string login_;
    std::string senha_;
};
