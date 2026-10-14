# ADR-0002: Renomear `nome` para `login` e acrescentar o papel ao usuário

- **Status:** aceito
- **Data:** 2026-10-14
- **Requisitos relacionados:** RF01, RF02, RF04

## Contexto

A classe `Usuario` tinha o campo `nome`, mas todas as regras de validação da Sprint 2 são regras de **login**, termo definido no glossário. O laboratório pede pelo menos dois tipos de usuário (Mestre e Jogador), que existiam só no diagrama de casos de uso.

## Decisão

O campo passa a se chamar `login` (`getLogin()`), e o usuário ganha um `Papel` (`MESTRE` ou `JOGADOR`), informado no cadastro e exibido na listagem. É uma mudança no contrato público de `Usuario` e do construtor de `ControleCriarConta`, registrada aqui conforme o CONTRIBUTING (§6.3).

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| Manter `nome` | Contradiz o glossário e as specs, que falam de login |
| Subclasses `Mestre` e `Jogador` de `Usuario` | Não há comportamento diferente entre os papéis nesta sprint; um enum basta e evita hierarquia prematura |

## Consequências

- Positivas: código, specs e glossário usam o mesmo termo; os dois tipos de usuário existem no sistema.
- Negativas: o arquivo `usuarios.txt` da versão anterior não é lido pela nova versão (o armazenamento passou a ser o arquivo binário `usuarios.dat`).
