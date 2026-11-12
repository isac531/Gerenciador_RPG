#pragma once

#include <string>
#include <vector>

#include "GeradorRelatorioAcessos.h"

// Subclasse concreta do Template Method: relatorio em PDF 1.4, montado
// sem biblioteca externa. Usa as fontes padrao do PDF (Helvetica e
// Courier), que todo leitor de PDF tem, e quebra em paginas A4.
//
// As operacoes primitivas so acumulam linhas; finalizarDocumento() monta
// os objetos do PDF (catalogo, paginas, fontes, conteudo) e a tabela xref.
class RelatorioPdf : public GeradorRelatorioAcessos {
public:
    std::string extensao() const override;

protected:
    void iniciarDocumento(const std::string& titulo) override;
    void escreverTitulo(const std::string& texto) override;
    void escreverSubtitulo(const std::string& texto) override;
    void escreverParagrafo(const std::string& texto) override;
    void escreverTabela(const Linha& cabecalho, const std::vector<Linha>& linhas) override;
    std::string finalizarDocumento() override;

private:
    enum class Fonte { NORMAL, NEGRITO, MONOESPACADA };

    struct LinhaTexto {
        Fonte fonte;
        int tamanho;
        std::string texto;
        double espacoAntes;
    };

    void adicionar(Fonte fonte, int tamanho, const std::string& texto, double espacoAntes = 0);

    // Conteudo (operadores de texto) de cada pagina.
    std::vector<std::string> paginar() const;

    std::string titulo_;
    std::vector<LinhaTexto> linhas_;
};
