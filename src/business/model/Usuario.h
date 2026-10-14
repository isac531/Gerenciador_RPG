#pragma once

#include <string>

class Usuario {
public:
    Usuario() = default;
    Usuario(int id, std::string nome, std::string senha);

    int getId() const;
    const std::string& getNome() const;
    const std::string& getSenha() const;

    // Retorna true se nome e senha informados correspondem a este usuario.
    bool fazerLogin(const std::string& nome, const std::string& senha) const;

private:
    int id_ = 0;
    std::string nome_;
    std::string senha_;
};
