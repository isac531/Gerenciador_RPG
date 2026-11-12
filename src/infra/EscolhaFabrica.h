#pragma once

#include <memory>

#include "../business/repository/FabricaRepositorio.h"

// Devolve a fabrica do mecanismo escolhido (RF05). E o unico lugar do
// sistema com um `if` sobre TipoArmazenamento: o restante do codigo so usa
// a classe abstrata FabricaRepositorio.
std::unique_ptr<FabricaRepositorio> escolherFabrica(TipoArmazenamento tipo);
