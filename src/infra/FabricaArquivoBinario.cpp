#include "FabricaArquivoBinario.h"

#include <utility>

#include "RepositorioArquivoBinario.h"
#include "RepositorioPersonagemArquivoBinario.h"

FabricaArquivoBinario::FabricaArquivoBinario(std::string caminhoUsuarios,
                                             std::string caminhoPersonagens)
    : caminhoUsuarios_(std::move(caminhoUsuarios)),
      caminhoPersonagens_(std::move(caminhoPersonagens)) {}

std::unique_ptr<IUsuarioRepositorio> FabricaArquivoBinario::criarRepositorioUsuarios() const {
    return std::make_unique<RepositorioArquivoBinario>(caminhoUsuarios_);
}

std::unique_ptr<IPersonagemRepositorio> FabricaArquivoBinario::criarRepositorioPersonagens() const {
    return std::make_unique<RepositorioPersonagemArquivoBinario>(caminhoPersonagens_);
}

TipoArmazenamento FabricaArquivoBinario::tipo() const {
    return TipoArmazenamento::ARQUIVO_BINARIO;
}
