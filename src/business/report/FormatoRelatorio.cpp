#include "FormatoRelatorio.h"

#include "RelatorioHtml.h"

std::optional<FormatoRelatorio> formatoDeCodigo(int codigo) {
    switch (codigo) {
    case 1: return FormatoRelatorio::HTML;
    default: return std::nullopt;
    }
}

std::unique_ptr<GeradorRelatorioAcessos> criarGeradorRelatorio(FormatoRelatorio formato) {
    (void)formato;  // por enquanto, so HTML
    return std::make_unique<RelatorioHtml>();
}
