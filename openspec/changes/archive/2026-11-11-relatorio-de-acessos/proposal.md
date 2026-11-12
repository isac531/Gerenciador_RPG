# Proposta: relatório de acessos (Sprint 4)

## Por quê

A Sprint 4 pede relatórios com estatísticas de acesso dos usuários, gerados na camada business com o padrão Template Method em mais de um formato (por exemplo, HTML e PDF). Hoje o sistema não registra acessos, porque o login (RF07, UC02) ainda não estava implementado. A mesma sprint pede Repository para todas as entidades, Factory Method para escolher o repositório e um cenário de Adapter (sugestão: biblioteca de log).

## O que muda

- **ADDED** RF13 Registrar acessos: toda tentativa de login, aceita ou recusada, é guardada com login, usuário, data e hora e resultado.
- **ADDED** RF14 Gerar relatório de acessos: estatísticas de acesso em HTML ou em PDF, à escolha do usuário.
- **MODIFIED** RF05: o armazenamento escolhido vale também para os registros de acesso (`acessos.dat`).
- **MODIFIED** NF-POR-01: passa a aceitar bibliotecas de terceiros copiadas em código-fonte no repositório (a biblioteca de log do Adapter), sem nada a instalar.
- Implementação do login (RF07, UC02) com a mensagem única "Login ou senha invalidos." (NF-SEG-02), porque sem login não há acessos para contar.
- Casos de uso: novo UC16 Gerar relatório de acessos.
- Código: entidade `RegistroAcesso` com `IAcessoRepositorio` e versões em memória e em arquivo; novo *factory method* `criarRepositorioAcessos()`; `ControleLogin`; `ControleRelatorioAcessos`; Template Method `GeradorRelatorioAcessos` com `RelatorioHtml` e `RelatorioPdf`; Adapter `LogCAdapter` para a biblioteca rxi/log.c.

## Fora do escopo

- Menus diferentes por papel depois do login (parte de RF07): o login autentica, registra o acesso e informa o papel, mas o menu continua o mesmo para Mestre e Jogador. Fica para quando houver funcionalidades exclusivas de cada papel (campanhas, escudo do mestre).
- Sessão de usuário (logout, ações em nome do usuário autenticado).
- Gráficos nos relatórios, filtro por período e acentos no PDF.
- Rotação ou consulta do arquivo de log `rpg.log` pelo sistema.

## Impacto

- Specs: nova capacidade `acessos`; `usuarios` (RF05) e `rnf/portabilidade` (NF-POR-01) modificadas.
- Documentos: casos de uso (UC16), `07-arquitetura/`, `08-projeto.md` (diagrama da S4.1 e tabela de padrões), [ADR-0005](../../../docs/07-arquitetura/adr/0005-repository-e-factory-method.md), [ADR-0006](../../../docs/07-arquitetura/adr/0006-adapter-log.md), [ADR-0007](../../../docs/07-arquitetura/adr/0007-relatorios-template-method.md).
