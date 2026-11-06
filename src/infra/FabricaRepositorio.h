#pragma once

#include <memory>
#include <string>

#include "../business/repository/IPersonagemRepositorio.h"
#include "../business/repository/IUsuarioRepositorio.h"
#include "../business/repository/TipoArmazenamento.h"

// Criam o repositorio escolhido. `caminho` so e usado pelo arquivo binario.
// Podem lancar FalhaArmazenamentoException (ex.: arquivo corrompido).
std::unique_ptr<IUsuarioRepositorio> criarRepositorioUsuarios(
    TipoArmazenamento tipo, const std::string& caminho = "usuarios.dat");

std::unique_ptr<IPersonagemRepositorio> criarRepositorioPersonagens(
    TipoArmazenamento tipo, const std::string& caminho = "personagens.dat");
