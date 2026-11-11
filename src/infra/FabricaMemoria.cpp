#include "FabricaMemoria.h"

#include "RepositorioMemoria.h"
#include "RepositorioPersonagemMemoria.h"

std::unique_ptr<IUsuarioRepositorio> FabricaMemoria::criarRepositorioUsuarios() const {
    return std::make_unique<RepositorioMemoria>();
}

std::unique_ptr<IPersonagemRepositorio> FabricaMemoria::criarRepositorioPersonagens() const {
    return std::make_unique<RepositorioPersonagemMemoria>();
}

TipoArmazenamento FabricaMemoria::tipo() const {
    return TipoArmazenamento::MEMORIA;
}
