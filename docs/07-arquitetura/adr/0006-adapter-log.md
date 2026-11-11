# ADR-0006: Adapter para a biblioteca de log

- **Status:** aceito
- **Data:** 2026-11-11
- **Requisitos relacionados:** RF07, RF13, NF-SEG-01, NF-MAN-01, NF-POR-01

## Contexto

A Sprint 4 pede um cenário do padrão **Adapter**, com a sugestão de usar uma biblioteca de log. O sistema não tinha log: cadastros, logins recusados e falhas de armazenamento só apareciam na tela e se perdiam. Com o login e o registro de acessos (RF07, RF13), passou a valer a pena registrar esses eventos num arquivo para auditoria.

Queremos usar uma biblioteca pronta, mas ela não pode "vazar" para a camada business: se a business chamar a biblioteca direto, trocar de biblioteca exige mexer em todos os controles, e a regra de dependência (`business` não depende de `infra`) é quebrada.

A biblioteca escolhida foi a [rxi/log.c](https://github.com/rxi/log.c) (MIT, 2 arquivos, C99, sem dependências). Ela compila no Linux e no Windows com MinGW (NF-POR-01), mas tem uma interface bem diferente da que a business precisa:

| O que a business quer (Target) | O que a log.c oferece (Adaptee) |
|---|---|
| Objeto injetável, com `info`, `aviso`, `erro` | Funções C globais: `log_log(nivel, arquivo, linha, fmt, ...)` |
| Mensagem `std::string` pronta | Formato estilo `printf` (um `%` na mensagem seria interpretado) |
| Gravar num arquivo, sem poluir o menu | Escreve em `stderr` por padrão; outros destinos via callback |
| Cabeçalho C++ | Cabeçalho C sem `extern "C"` |

## Decisão

Aplicar o **Adapter de objeto** (GoF):

- **Target:** `ILogger` em `business/log/ILogger.h`, com `info`, `aviso` e `erro`. A fachada recebe um `ILogger&` por `definirLogger()` e registra cadastros, logins aceitos e recusados, exclusões e falhas de armazenamento. Enquanto nenhum logger é definido, ela usa `LoggerNulo` (Null Object).
- **Adaptee:** a biblioteca log.c, copiada sem alterações em `src/infra/externo/logc/` e compilada como C pelo `Makefile`.
- **Adapter:** `LogCAdapter` em `infra/`, que implementa `ILogger` e traduz cada chamada para `log_log(LOG_INFO | LOG_WARN | LOG_ERROR, ..., "%s", mensagem)`. Ele inclui o cabeçalho dentro de `extern "C"`, desliga a saída em `stderr` (`log_set_quiet`) e registra um callback que grava no arquivo `rpg.log` no formato `AAAA-MM-DD hh:mm:ss NIVEL mensagem`.
- O `main.cpp` (raiz de composição) cria o `LogCAdapter` e o entrega à fachada; a `ui` e a `business` nunca incluem a log.c.
- A senha nunca é registrada (NF-SEG-01); há teste para isso.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| Chamar a log.c direto nos controles | A business passaria a depender de uma biblioteca da infra; trocar de biblioteca exigiria mudar todos os controles (fere DIP e OCP) |
| spdlog ou Boost.Log | Bibliotecas grandes, com dependências e configuração extra no Windows; a log.c resolve o problema com 2 arquivos |
| Escrever nosso próprio log com `std::ofstream` | Não haveria nada a adaptar; o objetivo da sprint é justamente integrar uma interface incompatível |
| Adapter de classe (herança múltipla) | A log.c não tem classe para herdar; é uma API de funções globais |

## Consequências

- Positivas: trocar a log.c por outra biblioteca exige só um novo adaptador; os testes da fachada usam um `ILogger` falso em memória, sem arquivo. Um login digitado com `%` não quebra o log, porque a mensagem vai como argumento de `"%s"`.
- Negativas: a log.c guarda os destinos em variáveis globais, então só um `LogCAdapter` fica ativo por vez (o `main.cpp` cria um só). O `Makefile` passa a compilar um arquivo C, o que exige `gcc` além de `g++` (já vem junto no MinGW). O arquivo `rpg.log` cresce sem limite; rotação de log ficou fora do escopo.
