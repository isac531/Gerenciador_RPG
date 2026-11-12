#include "RelatorioPdf.h"

#include <algorithm>
#include <cstdio>

namespace {

// Pagina A4 em pontos (1/72 de polegada).
constexpr double kLargura = 595;
constexpr double kAltura = 842;
constexpr double kMargem = 50;
constexpr double kEntrelinha = 1.4;
// Caracteres por linha que cabem na largura util em Courier 9 pt
// (cada caractere tem 0,6 do tamanho da fonte).
constexpr std::size_t kColunasCourier9 =
    static_cast<std::size_t>((kLargura - 2 * kMargem) / (9 * 0.6));

// Texto de uma string PDF: escapa \ ( ) e troca o que nao e ASCII por '?',
// ja que as fontes padrao usam WinAnsiEncoding.
std::string textoPdf(const std::string& texto) {
    std::string saida;
    for (unsigned char c : texto) {
        if (c == '\\' || c == '(' || c == ')') {
            saida += '\\';
            saida += static_cast<char>(c);
        } else if (c < 32 || c > 126) {
            saida += '?';
        } else {
            saida += static_cast<char>(c);
        }
    }
    return saida;
}

std::string completar(const std::string& texto, std::size_t largura) {
    return texto.size() >= largura ? texto : texto + std::string(largura - texto.size(), ' ');
}

std::string formatar(double valor) {
    char texto[32];
    std::snprintf(texto, sizeof(texto), "%.1f", valor);
    return texto;
}

}  // namespace

std::string RelatorioPdf::extensao() const { return "pdf"; }

void RelatorioPdf::adicionar(Fonte fonte, int tamanho, const std::string& texto,
                             double espacoAntes) {
    linhas_.push_back({fonte, tamanho, texto, espacoAntes});
}

void RelatorioPdf::iniciarDocumento(const std::string& titulo) {
    titulo_ = titulo;
    linhas_.clear();
}

void RelatorioPdf::escreverTitulo(const std::string& texto) {
    adicionar(Fonte::NEGRITO, 18, texto);
}

void RelatorioPdf::escreverSubtitulo(const std::string& texto) {
    adicionar(Fonte::NEGRITO, 13, texto, 10);
}

void RelatorioPdf::escreverParagrafo(const std::string& texto) {
    adicionar(Fonte::NORMAL, 10, texto, 4);
}

// Tabela em fonte monoespacada, com as colunas alinhadas por espacos.
void RelatorioPdf::escreverTabela(const Linha& cabecalho, const std::vector<Linha>& linhas) {
    std::vector<std::size_t> larguras(cabecalho.size(), 0);
    for (std::size_t i = 0; i < cabecalho.size(); ++i) {
        larguras[i] = cabecalho[i].size();
        for (const Linha& linha : linhas) {
            if (i < linha.size()) larguras[i] = std::max(larguras[i], linha[i].size());
        }
    }
    auto montar = [&](const Linha& celulas) {
        std::string texto;
        for (std::size_t i = 0; i < larguras.size(); ++i) {
            texto += completar(i < celulas.size() ? celulas[i] : "", larguras[i] + 2);
        }
        return texto.substr(0, kColunasCourier9);
    };

    std::size_t larguraTotal = 0;
    for (std::size_t largura : larguras) larguraTotal += largura + 2;

    adicionar(Fonte::MONOESPACADA, 9, montar(cabecalho), 4);
    adicionar(Fonte::MONOESPACADA, 9, std::string(std::min(larguraTotal, kColunasCourier9), '-'));
    for (const Linha& linha : linhas) {
        adicionar(Fonte::MONOESPACADA, 9, montar(linha));
    }
}

std::vector<std::string> RelatorioPdf::paginar() const {
    std::vector<std::string> paginas(1);
    double y = kAltura - kMargem;
    for (const LinhaTexto& linha : linhas_) {
        const double altura = linha.tamanho * kEntrelinha + linha.espacoAntes;
        if (y - altura < kMargem) {
            paginas.emplace_back();
            y = kAltura - kMargem;
        }
        y -= altura;
        const char* fonte = linha.fonte == Fonte::NEGRITO        ? "/F2"
                            : linha.fonte == Fonte::MONOESPACADA ? "/F3"
                                                                  : "/F1";
        paginas.back() += "BT " + std::string(fonte) + " " + std::to_string(linha.tamanho) +
                          " Tf " + formatar(kMargem) + " " + formatar(y) + " Td (" +
                          textoPdf(linha.texto) + ") Tj ET\n";
    }
    return paginas;
}

std::string RelatorioPdf::finalizarDocumento() {
    const std::vector<std::string> paginas = paginar();

    // Objetos: 1 catalogo, 2 arvore de paginas, 3 a 5 fontes, 6 informacoes,
    // depois um par (conteudo, pagina) para cada pagina.
    std::vector<std::string> objetos;
    std::string filhos;
    for (std::size_t i = 0; i < paginas.size(); ++i) {
        filhos += std::to_string(8 + 2 * i) + " 0 R ";
    }
    objetos.push_back("<< /Type /Catalog /Pages 2 0 R >>");
    objetos.push_back("<< /Type /Pages /Kids [ " + filhos + "] /Count " +
                      std::to_string(paginas.size()) + " >>");
    for (const char* nome : {"Helvetica", "Helvetica-Bold", "Courier"}) {
        objetos.push_back("<< /Type /Font /Subtype /Type1 /BaseFont /" + std::string(nome) +
                          " /Encoding /WinAnsiEncoding >>");
    }
    objetos.push_back("<< /Title (" + textoPdf(titulo_) + ") /Producer (Gerenciador de RPG) >>");
    for (std::size_t i = 0; i < paginas.size(); ++i) {
        objetos.push_back("<< /Length " + std::to_string(paginas[i].size()) + " >>\nstream\n" +
                          paginas[i] + "endstream");
        objetos.push_back("<< /Type /Page /Parent 2 0 R /MediaBox [0 0 595 842] "
                          "/Resources << /Font << /F1 3 0 R /F2 4 0 R /F3 5 0 R >> >> "
                          "/Contents " + std::to_string(7 + 2 * i) + " 0 R >>");
    }

    std::string pdf = "%PDF-1.4\n";
    std::vector<std::size_t> posicoes;
    for (std::size_t i = 0; i < objetos.size(); ++i) {
        posicoes.push_back(pdf.size());
        pdf += std::to_string(i + 1) + " 0 obj\n" + objetos[i] + "\nendobj\n";
    }

    const std::size_t inicioXref = pdf.size();
    pdf += "xref\n0 " + std::to_string(objetos.size() + 1) + "\n0000000000 65535 f \n";
    for (std::size_t posicao : posicoes) {
        char entrada[24];
        std::snprintf(entrada, sizeof(entrada), "%010lu 00000 n \n",
                      static_cast<unsigned long>(posicao));
        pdf += entrada;
    }
    pdf += "trailer\n<< /Size " + std::to_string(objetos.size() + 1) +
           " /Root 1 0 R /Info 6 0 R >>\nstartxref\n" + std::to_string(inicioXref) + "\n%%EOF\n";
    linhas_.clear();
    return pdf;
}
