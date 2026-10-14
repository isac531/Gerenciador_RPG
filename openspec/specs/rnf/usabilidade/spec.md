# rnf/usabilidade Specification

## Purpose
Requisitos não funcionais de facilidade de uso da interface e clareza das mensagens.

## Requirements

### Requirement: NF-USA-01 — Mensagem específica para cada recusa
O sistema DEVE informar, a cada cadastro recusado, qual regra foi violada, com uma mensagem diferente para cada uma das regras de RF01, RF02 e RF03.

**Prioridade:** Essencial
**Casos de uso:** UC01

#### Scenario: Uma recusa por regra
- **GIVEN** um conjunto de testes com uma entrada que viola cada regra (9 regras: ID não positivo, ID repetido, login repetido, 3 de login e 3 de senha)
- **WHEN** cada entrada é cadastrada
- **THEN** o sistema exibe 9 mensagens distintas, cada uma citando a regra violada

### Requirement: NF-USA-02 — Cadastro rápido de aprender
O sistema DEVE permitir que um usuário que nunca o usou cadastre uma conta e veja a listagem sem ajuda externa.

**Prioridade:** Importante
**Casos de uso:** UC01, UC03

#### Scenario: Primeiro uso
- **GIVEN** 5 jogadores de RPG que nunca usaram o sistema
- **WHEN** cada um tenta cadastrar uma conta válida e listar os usuários sem instrução
- **THEN** pelo menos 4 dos 5 concluem em até 2 minutos

### Requirement: NF-USA-03 — Entrada inválida no menu não encerra o sistema
O sistema DEVE tratar entradas inválidas nos menus (letras onde se espera número, opção inexistente) exibindo "Opcao invalida." e repetindo o menu.

**Prioridade:** Importante

#### Scenario: Letra no lugar da opção
- **WHEN** o usuário digita "abc" no menu principal
- **THEN** o sistema exibe "Opcao invalida." e o menu volta a aparecer, sem encerrar
