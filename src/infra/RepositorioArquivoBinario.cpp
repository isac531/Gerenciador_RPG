#include "RepositorioArquivoBinario.h"

#include <cstdint>
#include <utility>

#include "../business/exception/Excecoes.h"
#include "ArquivoBinario.h"

using namespace arquivo_binario;

RepositorioArquivoBinario::RepositorioArquivoBinario(std::string caminho)
    : caminho_(std::move(caminho)) {
    carregar();
}

const std::string& RepositorioArquivoBinario::getCaminho() const {
    return caminho_;
}

void RepositorioArquivoBinario::carregar() {
    std::map<int, Usuario> carregados;
    std::int32_t proximoId = 1;

    const bool existia = arquivo_binario::carregar(caminho_, [&](std::istream& arquivo) {
        proximoId = lerInt32(arquivo);
        const std::uint32_t quantidade = lerUint32(arquivo);
        for (std::uint32_t i = 0; i < quantidade; ++i) {
            const std::int32_t id = lerInt32(arquivo);
            std::string login = lerString(arquivo, caminho_);
            std::string senha = lerString(arquivo, caminho_);
            Papel papel;
            if (!papelDeCodigo(lerUint8(arquivo), papel)) {
                throw FalhaArmazenamentoException(
                    "Arquivo " + caminho_ + " corrompido: papel invalido.");
            }
            if (id <= 0 || id >= proximoId ||
                !carregados.emplace(id, Usuario(id, std::move(login), std::move(senha), papel)).second) {
                throw FalhaArmazenamentoException(
                    "Arquivo " + caminho_ + " corrompido: ID invalido ou duplicado.");
            }
        }
    });

    if (existia) {
        usuarios_ = std::move(carregados);
        proximoId_ = proximoId;
    }
}

void RepositorioArquivoBinario::gravar() const {
    arquivo_binario::gravar(caminho_, [this](std::ostream& arquivo) {
        escreverInt32(arquivo, proximoId_);
        escreverUint32(arquivo, static_cast<std::uint32_t>(usuarios_.size()));
        for (const auto& entrada : usuarios_) {
            const Usuario& usuario = entrada.second;
            escreverInt32(arquivo, usuario.getId());
            escreverString(arquivo, usuario.getLogin());
            escreverString(arquivo, usuario.getSenha());
            escreverUint8(arquivo, static_cast<std::uint8_t>(usuario.getPapel()));
        }
    });
}

int RepositorioArquivoBinario::adicionar(const Usuario& usuario) {
    const int id = proximoId_++;
    usuarios_.emplace(id, usuario.comId(id));
    try {
        gravar();
    } catch (...) {
        usuarios_.erase(id);
        --proximoId_;
        throw;
    }
    return id;
}

bool RepositorioArquivoBinario::remover(int id) {
    const auto encontrado = usuarios_.find(id);
    if (encontrado == usuarios_.end()) {
        return false;
    }

    const Usuario removido = encontrado->second;
    usuarios_.erase(encontrado);
    try {
        gravar();
    } catch (...) {
        usuarios_.emplace(id, removido);
        throw;
    }
    return true;
}

bool RepositorioArquivoBinario::existe(int id) const {
    return usuarios_.count(id) > 0;
}

std::optional<Usuario> RepositorioArquivoBinario::buscar(int id) const {
    const auto encontrado = usuarios_.find(id);
    if (encontrado == usuarios_.end()) {
        return std::nullopt;
    }
    return encontrado->second;
}

std::vector<Usuario> RepositorioArquivoBinario::listarTodos() const {
    std::vector<Usuario> todos;
    todos.reserve(usuarios_.size());
    for (const auto& entrada : usuarios_) {
        todos.push_back(entrada.second);
    }
    return todos;
}

std::size_t RepositorioArquivoBinario::quantidade() const {
    return usuarios_.size();
}
