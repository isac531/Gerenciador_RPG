#include "RepositorioMemoria.h"

int RepositorioMemoria::adicionar(const Usuario& usuario) {
    const int id = proximoId_++;
    usuarios_.emplace(id, usuario.comId(id));
    return id;
}

bool RepositorioMemoria::remover(int id) {
    return usuarios_.erase(id) > 0;
}

bool RepositorioMemoria::existe(int id) const {
    return usuarios_.count(id) > 0;
}

std::optional<Usuario> RepositorioMemoria::buscar(int id) const {
    const auto encontrado = usuarios_.find(id);
    if (encontrado == usuarios_.end()) {
        return std::nullopt;
    }
    return encontrado->second;
}

std::optional<Usuario> RepositorioMemoria::buscarPorLogin(const std::string& login) const {
    for (const auto& entrada : usuarios_) {
        if (entrada.second.getLogin() == login) {
            return entrada.second;
        }
    }
    return std::nullopt;
}

std::vector<Usuario> RepositorioMemoria::listarTodos() const {
    std::vector<Usuario> todos;
    todos.reserve(usuarios_.size());
    for (const auto& entrada : usuarios_) {
        todos.push_back(entrada.second);
    }
    return todos;
}

std::size_t RepositorioMemoria::quantidade() const {
    return usuarios_.size();
}
