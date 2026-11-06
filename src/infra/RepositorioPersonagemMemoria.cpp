#include "RepositorioPersonagemMemoria.h"

int RepositorioPersonagemMemoria::adicionar(const Personagem& personagem) {
    const int id = estado_.proximoId++;
    estado_.personagens.emplace(id, personagem.comId(id));
    return id;
}

bool RepositorioPersonagemMemoria::atualizar(const Personagem& personagem) {
    const auto encontrado = estado_.personagens.find(personagem.getId());
    if (encontrado == estado_.personagens.end()) {
        return false;
    }
    encontrado->second = personagem;
    return true;
}

bool RepositorioPersonagemMemoria::remover(int id) {
    return estado_.personagens.erase(id) > 0;
}

std::optional<Personagem> RepositorioPersonagemMemoria::buscar(int id) const {
    const auto encontrado = estado_.personagens.find(id);
    if (encontrado == estado_.personagens.end()) {
        return std::nullopt;
    }
    return encontrado->second;
}

std::vector<Personagem> RepositorioPersonagemMemoria::listarTodos() const {
    std::vector<Personagem> todos;
    todos.reserve(estado_.personagens.size());
    for (const auto& entrada : estado_.personagens) {
        todos.push_back(entrada.second);
    }
    return todos;
}

std::size_t RepositorioPersonagemMemoria::quantidade() const {
    return estado_.personagens.size();
}
