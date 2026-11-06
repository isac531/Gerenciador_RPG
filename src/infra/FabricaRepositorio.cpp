#include "FabricaRepositorio.h"

#include "RepositorioArquivoBinario.h"
#include "RepositorioMemoria.h"
#include "RepositorioPersonagemArquivoBinario.h"
#include "RepositorioPersonagemMemoria.h"

std::unique_ptr<IUsuarioRepositorio> criarRepositorioUsuarios(
    TipoArmazenamento tipo, const std::string& caminho) {
    if (tipo == TipoArmazenamento::ARQUIVO_BINARIO) {
        return std::make_unique<RepositorioArquivoBinario>(caminho);
    }
    return std::make_unique<RepositorioMemoria>();
}

std::unique_ptr<IPersonagemRepositorio> criarRepositorioPersonagens(
    TipoArmazenamento tipo, const std::string& caminho) {
    if (tipo == TipoArmazenamento::ARQUIVO_BINARIO) {
        return std::make_unique<RepositorioPersonagemArquivoBinario>(caminho);
    }
    return std::make_unique<RepositorioPersonagemMemoria>();
}
