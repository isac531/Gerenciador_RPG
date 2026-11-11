#pragma once

#include "../business/repository/FabricaRepositorio.h"

// Creator concreto: produz repositorios que guardam tudo na RAM.
class FabricaMemoria : public FabricaRepositorio {
public:
    std::unique_ptr<IUsuarioRepositorio> criarRepositorioUsuarios() const override;
    std::unique_ptr<IPersonagemRepositorio> criarRepositorioPersonagens() const override;
    TipoArmazenamento tipo() const override;
};
