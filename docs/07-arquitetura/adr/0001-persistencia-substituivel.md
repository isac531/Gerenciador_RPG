# ADR-0001: Tornar o mecanismo de armazenamento substituível

- **Status:** aceito
- **Data:** 2026-10-14
- **Requisitos relacionados:** RF05, NF-MAN-01, NF-CON-01, NF-CON-02

## Contexto

Na Sprint 1 os usuários ficavam só numa coleção em memória. Uma primeira versão da Sprint 2 passou a gravar um arquivo de texto diretamente dentro de `ControleCriarConta`, misturando validação de negócio com leitura e escrita de arquivo e sem permitir voltar ao modo em memória. A Sprint 2 pede dois mecanismos (RAM e arquivo binário ou banco), escolhidos no início da execução, com tratamento das exceções de E/S.

## Decisão

Os controles (`ControleCriarConta` e `ControleUsuarios`) dependem apenas da interface `IUsuarioRepositorio`. Há duas implementações: `RepositorioMemoria` e `RepositorioArquivoBinario` (`usuarios.dat`). A escolha é feita uma única vez, no início da execução, e o repositório é entregue aos controles pelo construtor (injeção de dependência); o `main.cpp` é o único que conhece as classes concretas. Erros de E/S (`std::ios_base::failure`, arquivo truncado ou corrompido) são convertidos em `FalhaArmazenamentoException`, tratada na interface sem encerrar o programa.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| Manter a gravação dentro do controle com `if (modo == ARQUIVO)` | Mistura regra de negócio com detalhe de infraestrutura e espalha a decisão (viola SRP e OCP) |
| Arquivo de texto em vez de binário | A sprint pede arquivo binário ou banco; texto exige escapar aspas e espaços |
| Banco de dados (SQLite) | Exige biblioteca externa e configuração no Windows (NF-POR-01) |

## Consequências

- Positivas: os testes de regras de negócio rodam em memória; acrescentar um banco de dados depois só exige uma nova implementação da interface. Prepara o padrão Repository e a fábrica de repositórios da Sprint 4.
- Negativas: uma interface a mais para manter; o arquivo binário não é legível a olho e as senhas ficam sem criptografia.
