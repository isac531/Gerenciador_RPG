#include "Usuario.h"

#include <utility>

Usuario::Usuario(int id, std::string login, std::string senha)
    : id_(id), login_(std::move(login)), senha_(std::move(senha)) {}

int Usuario::getId() const {
    return id_;
}

const std::string& Usuario::getLogin() const {
    return login_;
}

const std::string& Usuario::getSenha() const {
    return senha_;
}

bool Usuario::fazerLogin(const std::string& login, const std::string& senha) const {
    return login == login_ && senha == senha_;
}
