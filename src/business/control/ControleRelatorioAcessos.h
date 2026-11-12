#pragma once

#include <string>

#include "../report/EstatisticasAcesso.h"
#include "../report/FormatoRelatorio.h"
#include "../repository/IAcessoRepositorio.h"
#include "../repository/IUsuarioRepositorio.h"
#include "ControleLogin.h"

// Documento de relatorio pronto para gravar.
struct RelatorioGerado {
    std::string conteudo;
    std::string extensao;  // sem ponto: "html" ou "pdf"
};

// Controle do caso de uso UC16 Gerar relatorio de acessos (RF14): calcula
// as estatisticas a partir dos repositorios e entrega ao gerador do formato.
class ControleRelatorioAcessos {
public:
    ControleRelatorioAcessos(const IUsuarioRepositorio& usuarios,
                             const IAcessoRepositorio& acessos,
                             ControleLogin::Relogio relogio = ControleLogin::agora);

    EstatisticasAcesso calcularEstatisticas() const;

    RelatorioGerado gerar(FormatoRelatorio formato) const;

private:
    const IUsuarioRepositorio& usuarios_;
    const IAcessoRepositorio& acessos_;
    ControleLogin::Relogio relogio_;
};
