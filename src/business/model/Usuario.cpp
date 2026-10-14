#include "Usuario.h"

#include <utility>

Usuario::Usuario(int id, std::string nome, std::string senha)
    : id_(id), nome_(std::move(nome)), senha_(std::move(senha)) {}

int Usuario::getId() const {
    return id_;
}

const std::string& Usuario::getNome() const {
    return nome_;
}

const std::string& Usuario::getSenha() const {
    return senha_;
}

bool Usuario::fazerLogin(const std::string& nome, const std::string& senha) const {
    return nome == nome_ && senha == senha_;
}
