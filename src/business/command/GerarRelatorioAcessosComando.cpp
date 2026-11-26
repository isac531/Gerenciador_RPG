#include "GerarRelatorioAcessosComando.h"

GerarRelatorioAcessosComando::GerarRelatorioAcessosComando(
    const ControleRelatorioAcessos& controle, FormatoRelatorio formato, ILogger& logger)
    : controle_(controle), formato_(formato), logger_(logger) {}

std::string GerarRelatorioAcessosComando::descricaoFalha() const {
    return "Falha ao gerar relatorio de acessos";
}

RelatorioGerado GerarRelatorioAcessosComando::executarComResultado() {
    RelatorioGerado relatorio = controle_.gerar(formato_);
    logger_.info("Relatorio de acessos gerado em " + relatorio.extensao);
    return relatorio;
}
