#include "RelatorioHtml.h"

#include <utility>

std::string RelatorioHtml::extensao() const { return "html"; }

std::string RelatorioHtml::escapar(const std::string& texto) {
    std::string escapado;
    for (char c : texto) {
        switch (c) {
        case '&': escapado += "&amp;"; break;
        case '<': escapado += "&lt;"; break;
        case '>': escapado += "&gt;"; break;
        case '"': escapado += "&quot;"; break;
        case '\'': escapado += "&#39;"; break;
        default: escapado += c;
        }
    }
    return escapado;
}

void RelatorioHtml::iniciarDocumento(const std::string& titulo) {
    html_ = "<!DOCTYPE html>\n<html lang=\"pt-BR\">\n<head>\n<meta charset=\"utf-8\">\n"
            "<title>" + escapar(titulo) + "</title>\n"
            "<style>\n"
            "body { font-family: sans-serif; margin: 2rem; color: #222; }\n"
            "table { border-collapse: collapse; margin-bottom: 1.5rem; }\n"
            "th, td { border: 1px solid #999; padding: 4px 10px; text-align: left; }\n"
            "th { background: #eee; }\n"
            "</style>\n</head>\n<body>\n";
}

void RelatorioHtml::escreverTitulo(const std::string& texto) {
    html_ += "<h1>" + escapar(texto) + "</h1>\n";
}

void RelatorioHtml::escreverSubtitulo(const std::string& texto) {
    html_ += "<h2>" + escapar(texto) + "</h2>\n";
}

void RelatorioHtml::escreverParagrafo(const std::string& texto) {
    html_ += "<p>" + escapar(texto) + "</p>\n";
}

void RelatorioHtml::escreverTabela(const Linha& cabecalho, const std::vector<Linha>& linhas) {
    html_ += "<table>\n<tr>";
    for (const std::string& coluna : cabecalho) {
        html_ += "<th>" + escapar(coluna) + "</th>";
    }
    html_ += "</tr>\n";
    for (const Linha& linha : linhas) {
        html_ += "<tr>";
        for (const std::string& celula : linha) {
            html_ += "<td>" + escapar(celula) + "</td>";
        }
        html_ += "</tr>\n";
    }
    html_ += "</table>\n";
}

std::string RelatorioHtml::finalizarDocumento() {
    html_ += "</body>\n</html>\n";
    return std::move(html_);
}
