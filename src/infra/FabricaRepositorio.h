#pragma once

#include <memory>
#include <string>

#include "../business/repository/IUsuarioRepositorio.h"

enum class TipoArmazenamento {
    MEMORIA,
    ARQUIVO_BINARIO
};

// Cria o repositorio escolhido. `caminho` so e usado pelo arquivo binario.
// Pode lancar FalhaArmazenamentoException (ex.: arquivo corrompido).
std::unique_ptr<IUsuarioRepositorio> criarRepositorio(
    TipoArmazenamento tipo, const std::string& caminho = "usuarios.dat");
