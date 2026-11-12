#pragma once

#include <string>

#include "GeradorRelatorioAcessos.h"

// Subclasse concreta do Template Method: relatorio em HTML, uma pagina
// unica com estilo embutido, que abre em qualquer navegador.
class RelatorioHtml : public GeradorRelatorioAcessos {
public:
    std::string extensao() const override;

    // Troca & < > " ' pelas entidades HTML (um login nunca vira tag).
    static std::string escapar(const std::string& texto);

protected:
    void iniciarDocumento(const std::string& titulo) override;
    void escreverTitulo(const std::string& texto) override;
    void escreverSubtitulo(const std::string& texto) override;
    void escreverParagrafo(const std::string& texto) override;
    void escreverTabela(const Linha& cabecalho, const std::vector<Linha>& linhas) override;
    std::string finalizarDocumento() override;

private:
    std::string html_;
};
