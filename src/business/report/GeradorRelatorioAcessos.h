#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "EstatisticasAcesso.h"

// Padrao Template Method (GoF), ADR-0007.
//
// gerar() e o metodo-template: define, uma unica vez, QUAIS secoes o
// relatorio de acessos tem e EM QUE ORDEM (titulo, data de geracao, resumo,
// acessos por usuario). Os passos que dependem do formato sao operacoes
// primitivas abstratas, implementadas pelas subclasses (RelatorioHtml,
// RelatorioPdf). Um formato novo = uma subclasse nova; o conteudo do
// relatorio nao muda.
//
// gerar() nao e virtual de proposito: as subclasses nao podem alterar o
// algoritmo, so os passos.
class GeradorRelatorioAcessos {
public:
    using Linha = std::vector<std::string>;

    virtual ~GeradorRelatorioAcessos() = default;

    // Metodo-template. Retorna o documento completo, pronto para gravar.
    std::string gerar(const EstatisticasAcesso& estatisticas);

    // Extensao do arquivo, sem ponto (ex.: "html").
    virtual std::string extensao() const = 0;

    // Data e hora local no formato "dd/mm/aaaa hh:mm".
    static std::string formatarDataHora(std::int64_t segundosDesde1970);

protected:
    // ----- Operacoes primitivas (cada formato implementa) -----
    virtual void iniciarDocumento(const std::string& titulo) = 0;
    virtual void escreverTitulo(const std::string& texto) = 0;
    virtual void escreverSubtitulo(const std::string& texto) = 0;
    virtual void escreverParagrafo(const std::string& texto) = 0;
    virtual void escreverTabela(const Linha& cabecalho, const std::vector<Linha>& linhas) = 0;
    virtual std::string finalizarDocumento() = 0;

    // ----- Gancho (hook): as subclasses podem sobrescrever -----
    // Texto do rodape; o padrao identifica o sistema.
    virtual std::string textoRodape() const;

private:
    // Passos fixos do algoritmo, iguais para todos os formatos.
    void escreverResumo(const EstatisticasAcesso& estatisticas);
    void escreverAcessosPorUsuario(const EstatisticasAcesso& estatisticas);
};
