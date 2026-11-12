#pragma once

#include <memory>
#include <optional>

#include "GeradorRelatorioAcessos.h"

// Formatos do relatorio de acessos (RF14).
enum class FormatoRelatorio { HTML = 1, PDF = 2 };

// Converte o codigo digitado no menu; std::nullopt se nao existir.
std::optional<FormatoRelatorio> formatoDeCodigo(int codigo);

// Cria o gerador (subclasse do Template Method) do formato pedido.
std::unique_ptr<GeradorRelatorioAcessos> criarGeradorRelatorio(FormatoRelatorio formato);
