#include "FabricaRepositorio.h"

#include "RepositorioArquivoBinario.h"
#include "RepositorioMemoria.h"

std::unique_ptr<IUsuarioRepositorio> criarRepositorio(
    TipoArmazenamento tipo, const std::string& caminho) {
    switch (tipo) {
    case TipoArmazenamento::ARQUIVO_BINARIO:
        return std::make_unique<RepositorioArquivoBinario>(caminho);
    case TipoArmazenamento::MEMORIA:
        break;
    }
    return std::make_unique<RepositorioMemoria>();
}
