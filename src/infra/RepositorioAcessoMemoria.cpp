#include "RepositorioAcessoMemoria.h"

int RepositorioAcessoMemoria::adicionar(const RegistroAcesso& registro) {
    const int id = estado_.proximoId++;
    estado_.registros.emplace(id, registro.comId(id));
    return id;
}

std::vector<RegistroAcesso> RepositorioAcessoMemoria::listarTodos() const {
    std::vector<RegistroAcesso> todos;
    todos.reserve(estado_.registros.size());
    for (const auto& entrada : estado_.registros) {
        todos.push_back(entrada.second);
    }
    return todos;
}

std::vector<RegistroAcesso> RepositorioAcessoMemoria::listarPorLogin(
    const std::string& login) const {
    std::vector<RegistroAcesso> doLogin;
    for (const auto& entrada : estado_.registros) {
        if (entrada.second.getLogin() == login) {
            doLogin.push_back(entrada.second);
        }
    }
    return doLogin;
}

std::size_t RepositorioAcessoMemoria::quantidade() const {
    return estado_.registros.size();
}
