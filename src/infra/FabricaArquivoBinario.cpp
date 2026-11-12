#include "FabricaArquivoBinario.h"

#include <utility>

#include "RepositorioAcessoArquivoBinario.h"
#include "RepositorioArquivoBinario.h"
#include "RepositorioPersonagemArquivoBinario.h"

FabricaArquivoBinario::FabricaArquivoBinario(std::string caminhoUsuarios,
                                             std::string caminhoPersonagens,
                                             std::string caminhoAcessos)
    : caminhoUsuarios_(std::move(caminhoUsuarios)),
      caminhoPersonagens_(std::move(caminhoPersonagens)),
      caminhoAcessos_(std::move(caminhoAcessos)) {}

std::unique_ptr<IUsuarioRepositorio> FabricaArquivoBinario::criarRepositorioUsuarios() const {
    return std::make_unique<RepositorioArquivoBinario>(caminhoUsuarios_);
}

std::unique_ptr<IPersonagemRepositorio> FabricaArquivoBinario::criarRepositorioPersonagens() const {
    return std::make_unique<RepositorioPersonagemArquivoBinario>(caminhoPersonagens_);
}

std::unique_ptr<IAcessoRepositorio> FabricaArquivoBinario::criarRepositorioAcessos() const {
    return std::make_unique<RepositorioAcessoArquivoBinario>(caminhoAcessos_);
}

TipoArmazenamento FabricaArquivoBinario::tipo() const {
    return TipoArmazenamento::ARQUIVO_BINARIO;
}
