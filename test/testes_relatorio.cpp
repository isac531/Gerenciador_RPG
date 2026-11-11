#include <string>
#include <vector>

#include "../src/business/control/ControleRelatorioAcessos.h"
#include "../src/business/control/FacadeSingletonController.h"
#include "../src/business/report/GeradorRelatorioAcessos.h"
#include "../src/business/report/RelatorioHtml.h"
#include "../src/business/report/RelatorioPdf.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "TestRunner.h"

namespace {

constexpr std::int64_t kInstante = 1794398400;  // 11/11/2026 12:00:00 UTC

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}

// isac (Mestre): 2 acessos e 1 recusa; bia (Jogador): nenhum acesso;
// 1 tentativa com login inexistente.
struct CenarioRelatorio {
    RepositorioMemoria usuarios;
    RepositorioAcessoMemoria acessos;
    ControleRelatorioAcessos controle{usuarios, acessos, [] { return kInstante + 3600; }};

    CenarioRelatorio() {
        const int isac = usuarios.adicionar(Usuario("isac", "Dado#2026", Papel::MESTRE));
        usuarios.adicionar(Usuario("bia", "Senha@123", Papel::JOGADOR));
        acessos.adicionar(RegistroAcesso("isac", isac, kInstante, true));
        acessos.adicionar(RegistroAcesso("isac", isac, kInstante + 60, false));
        acessos.adicionar(RegistroAcesso("isac", isac, kInstante + 120, true));
        acessos.adicionar(RegistroAcesso("x<b>", SEM_ID, kInstante + 180, false));
    }
};

// Subclasse de teste que so anota a ordem das operacoes primitivas.
class GeradorEspiao : public GeradorRelatorioAcessos {
public:
    std::string extensao() const override { return "txt"; }
    std::vector<std::string> chamadas;

protected:
    void iniciarDocumento(const std::string&) override { chamadas.push_back("iniciar"); }
    void escreverTitulo(const std::string&) override { chamadas.push_back("titulo"); }
    void escreverSubtitulo(const std::string& texto) override {
        chamadas.push_back("subtitulo:" + texto);
    }
    void escreverParagrafo(const std::string&) override { chamadas.push_back("paragrafo"); }
    void escreverTabela(const Linha&, const std::vector<Linha>&) override {
        chamadas.push_back("tabela");
    }
    std::string finalizarDocumento() override {
        chamadas.push_back("finalizar");
        return "ok";
    }
};

// Confere que cada entrada da tabela xref aponta para "N 0 obj".
bool xrefConfere(const std::string& pdf) {
    const std::size_t inicio = pdf.find("xref\n");
    if (inicio == std::string::npos) return false;
    std::size_t posicao = pdf.find('\n', pdf.find('\n', inicio + 5) + 1) + 1;  // pula "0 N" e a entrada livre
    for (int objeto = 1; pdf.compare(posicao, 7, "trailer") != 0; ++objeto) {
        const std::size_t deslocamento = std::stoul(pdf.substr(posicao, 10));
        const std::string esperado = std::to_string(objeto) + " 0 obj";
        if (pdf.compare(deslocamento, esperado.size(), esperado) != 0) return false;
        posicao += 20;
    }
    return std::to_string(inicio) ==
           pdf.substr(pdf.find("startxref\n") + 10, std::to_string(inicio).size());
}

}  // namespace

// ---------- Estatisticas de acesso (RF14) ----------

TESTE(estatisticas_devem_contar_tentativas_aceitas_e_recusadas) {
    CenarioRelatorio cenario;
    const EstatisticasAcesso e = cenario.controle.calcularEstatisticas();
    VERIFICAR(e.totalTentativas == 4);
    VERIFICAR(e.acessosAceitos == 2);
    VERIFICAR(e.tentativasRecusadas == 2);
    VERIFICAR(e.tentativasLoginInexistente == 1);
    VERIFICAR(e.taxaDeSucesso() == 50.0);
    VERIFICAR(*e.primeiraTentativa == kInstante);
    VERIFICAR(*e.ultimaTentativa == kInstante + 180);
    VERIFICAR(e.geradoEm == kInstante + 3600);
}

TESTE(estatisticas_devem_listar_todos_os_usuarios_com_seus_acessos) {
    CenarioRelatorio cenario;
    const EstatisticasAcesso e = cenario.controle.calcularEstatisticas();
    VERIFICAR(e.usuariosCadastrados == 2);
    VERIFICAR(e.usuariosQueAcessaram == 1);
    VERIFICAR(e.porUsuario.size() == 2);
    VERIFICAR(e.porUsuario[0].login == "isac");
    VERIFICAR(e.porUsuario[0].acessos == 2);
    VERIFICAR(e.porUsuario[0].recusas == 1);
    VERIFICAR(*e.porUsuario[0].ultimoAcesso == kInstante + 120);
    VERIFICAR(e.porUsuario[1].acessos == 0);
    VERIFICAR(!e.porUsuario[1].ultimoAcesso);
    VERIFICAR(e.usuarioMaisAtivo()->login == "isac");
}

TESTE(estatisticas_sem_acessos_devem_ficar_zeradas) {
    RepositorioMemoria usuarios;
    RepositorioAcessoMemoria acessos;
    const EstatisticasAcesso e = ControleRelatorioAcessos(usuarios, acessos).calcularEstatisticas();
    VERIFICAR(e.totalTentativas == 0);
    VERIFICAR(e.taxaDeSucesso() == 0.0);
    VERIFICAR(!e.usuarioMaisAtivo());
    VERIFICAR(!e.primeiraTentativa);
}

// ---------- Template Method: o algoritmo e fixo, os passos variam ----------

TESTE(template_method_deve_chamar_os_passos_sempre_na_mesma_ordem) {
    CenarioRelatorio cenario;
    GeradorEspiao gerador;
    VERIFICAR(gerador.gerar(cenario.controle.calcularEstatisticas()) == "ok");
    const std::vector<std::string> esperado = {
        "iniciar",   "titulo", "paragrafo", "subtitulo:Resumo", "tabela",
        "subtitulo:Acessos por usuario", "tabela", "paragrafo", "finalizar"};
    VERIFICAR(gerador.chamadas == esperado);
}

TESTE(relatorio_html_deve_conter_resumo_e_tabela_por_usuario) {
    CenarioRelatorio cenario;
    RelatorioHtml html;
    const std::string documento = html.gerar(cenario.controle.calcularEstatisticas());
    VERIFICAR(html.extensao() == "html");
    VERIFICAR(documento.rfind("<!DOCTYPE html>", 0) == 0);
    VERIFICAR(contem(documento, "<h1>Relatorio de acessos</h1>"));
    VERIFICAR(contem(documento, "<td>Taxa de sucesso</td><td>50.0%</td>"));
    VERIFICAR(contem(documento, "<td>isac</td><td>Mestre</td><td>2</td><td>1</td>"));
    VERIFICAR(contem(documento, "<td>bia</td><td>Jogador</td><td>0</td><td>0</td><td>-</td>"));
    VERIFICAR(contem(documento, "</html>"));
}

TESTE(relatorio_html_deve_escapar_caracteres_especiais) {
    VERIFICAR(RelatorioHtml::escapar("<b>&\"'") == "&lt;b&gt;&amp;&quot;&#39;");
}

TESTE(relatorio_pdf_deve_ser_um_pdf_valido_com_os_dados) {
    CenarioRelatorio cenario;
    RelatorioPdf pdf;
    const std::string documento = pdf.gerar(cenario.controle.calcularEstatisticas());
    VERIFICAR(pdf.extensao() == "pdf");
    VERIFICAR(documento.rfind("%PDF-1.4\n", 0) == 0);
    VERIFICAR(contem(documento, "/BaseFont /Helvetica"));
    VERIFICAR(contem(documento, "(Relatorio de acessos) Tj"));
    VERIFICAR(contem(documento, "Taxa de sucesso"));
    VERIFICAR(contem(documento, "isac"));
    VERIFICAR(documento.size() > 6 && documento.compare(documento.size() - 6, 6, "%%EOF\n") == 0);
    VERIFICAR(xrefConfere(documento));
}

TESTE(relatorio_pdf_com_muitos_usuarios_deve_quebrar_em_paginas) {
    RepositorioMemoria usuarios;
    RepositorioAcessoMemoria acessos;
    for (int i = 0; i < 120; ++i) {
        usuarios.adicionar(Usuario("jogador", "Senha@123", Papel::JOGADOR));
    }
    RelatorioPdf pdf;
    const std::string documento =
        pdf.gerar(ControleRelatorioAcessos(usuarios, acessos).calcularEstatisticas());
    VERIFICAR(!contem(documento, "/Count 1 "));
    VERIFICAR(xrefConfere(documento));
}

TESTE(fachada_deve_gerar_relatorio_no_formato_pedido) {
    RepositorioMemoria usuarios;
    RepositorioPersonagemMemoria personagens;
    RepositorioAcessoMemoria acessos;
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    fachada.inicializar(usuarios, personagens, acessos);
    VERIFICAR(fachada.gerarRelatorioAcessos(FormatoRelatorio::HTML).extensao == "html");
    VERIFICAR(fachada.gerarRelatorioAcessos(FormatoRelatorio::PDF).extensao == "pdf");
    VERIFICAR(formatoDeCodigo(2) == FormatoRelatorio::PDF);
    VERIFICAR(!formatoDeCodigo(3));
}
