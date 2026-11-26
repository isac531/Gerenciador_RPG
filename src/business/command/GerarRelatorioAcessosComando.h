#pragma once

#include "../control/ControleRelatorioAcessos.h"
#include "../log/ILogger.h"
#include "../report/FormatoRelatorio.h"
#include "ComandoComResultado.h"

// Comando do UC16 Gerar relatorio de acessos (RF14).
// Resultado: o conteudo e a extensao do documento.
class GerarRelatorioAcessosComando : public ComandoComResultado<RelatorioGerado> {
public:
    GerarRelatorioAcessosComando(const ControleRelatorioAcessos& controle,
                                 FormatoRelatorio formato, ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    RelatorioGerado executarComResultado() override;

private:
    const ControleRelatorioAcessos& controle_;
    FormatoRelatorio formato_;
    ILogger& logger_;
};
