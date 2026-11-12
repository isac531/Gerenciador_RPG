#include "FormatoRelatorio.h"

#include "RelatorioHtml.h"
#include "RelatorioPdf.h"

std::optional<FormatoRelatorio> formatoDeCodigo(int codigo) {
    switch (codigo) {
    case 1: return FormatoRelatorio::HTML;
    case 2: return FormatoRelatorio::PDF;
    default: return std::nullopt;
    }
}

std::unique_ptr<GeradorRelatorioAcessos> criarGeradorRelatorio(FormatoRelatorio formato) {
    if (formato == FormatoRelatorio::PDF) {
        return std::make_unique<RelatorioPdf>();
    }
    return std::make_unique<RelatorioHtml>();
}
