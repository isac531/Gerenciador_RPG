#include "RepositorioMemoria.h"

bool RepositorioMemoria::adicionar(const Usuario& usuario) {
    return usuarios_.emplace(usuario.getId(), usuario).second;
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

std::vector<Usuario> RepositorioMemoria::listarTodos() const {
    std::vector<Usuario> todos;
    todos.reserve(usuarios_.size());
    for (const auto& entrada : usuarios_) {
        todos.push_back(entrada.second);
    }
    return todos;
}
