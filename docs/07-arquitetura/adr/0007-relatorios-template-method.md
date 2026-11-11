# ADR-0007: Template Method para os relatórios de acesso

- **Status:** aceito
- **Data:** 2026-11-11
- **Requisitos relacionados:** RF13, RF14, NF-MAN-01, NF-POR-01

## Contexto

A Sprint 4 pede que a camada business gere, com o padrão **Template Method**, relatórios de estatísticas de acesso dos usuários em mais de um formato (por exemplo, HTML e PDF). Os acessos passaram a ser registrados pelo login (RF13, [ADR-0005](0005-repository-e-factory-method.md)).

Os formatos diferem só na forma de escrever cada parte (tag HTML, operador de texto do PDF). O conteúdo e a ordem das seções (título, data, resumo, acessos por usuário, rodapé) têm de ser iguais em todos. Se cada formato montasse o relatório inteiro, a ordem e o cálculo das seções seriam duplicados e poderiam divergir.

## Decisão

- **Classe abstrata:** `GeradorRelatorioAcessos` (`business/report`). O método-template `gerar(estatisticas)` **não é virtual** e fixa o algoritmo: `iniciarDocumento` → `escreverTitulo` → data de geração → seção "Resumo" → seção "Acessos por usuário" → rodapé → `finalizarDocumento`. Os passos "Resumo" e "Acessos por usuário" são privados e montam as linhas uma única vez para todos os formatos.
- **Operações primitivas** (abstratas): `iniciarDocumento`, `escreverTitulo`, `escreverSubtitulo`, `escreverParagrafo`, `escreverTabela`, `finalizarDocumento` e `extensao`.
- **Gancho (hook):** `textoRodape()`, com implementação padrão que as subclasses podem trocar.
- **Subclasses concretas:**
  - `RelatorioHtml`: página única com CSS embutido; escapa `& < > " '`.
- `EstatisticasAcesso` guarda os números, independentes de formato, e `ControleRelatorioAcessos` os calcula a partir de `IUsuarioRepositorio` e `IAcessoRepositorio`. `criarGeradorRelatorio(formato)` escolhe a subclasse. A fachada expõe `gerarRelatorioAcessos(formato)`, que devolve o conteúdo e a extensão. O menu grava `relatorio-acessos.<extensão>`.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| Uma função por formato montando o relatório inteiro | Duplica a ordem e o conteúdo das seções; um indicador novo teria de ser incluído em todos os formatos |
| Strategy (um objeto "formatador" injetado num gerador concreto) | Também funcionaria, mas a sprint pede Template Method; aqui o algoritmo é fixo e só os passos variam, que é o caso típico do padrão |

## Consequências

- Positivas: um formato novo (CSV, texto) é uma subclasse com seis métodos, sem tocar no conteúdo do relatório. Um indicador novo é incluído uma vez em `GeradorRelatorioAcessos` e aparece em todos os formatos. Um teste com uma subclasse espiã confere a ordem dos passos.
- Negativas: as subclasses guardam estado entre os passos, por isso um mesmo gerador não deve ser usado por duas threads ao mesmo tempo.
