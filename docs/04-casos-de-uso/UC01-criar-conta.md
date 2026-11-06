# [UC01] Criar conta

Este caso de uso é responsável pelo cadastro de um Visitante como Mestre ou Jogador, com login e senha válidos.

| Campo | Valor |
|---|---|
| **Ator** | Visitante |
| **Prioridade** | Essencial |
| **Requisitos** | RF01, RF02, RF03, RF05, NF-SEG-01, NF-USA-01, NF-CON-01 |
| **Interfaces** | TL01, TL02 |

**Entradas e pré-condições**
- O armazenamento (memória ou arquivo binário) já foi escolhido ao iniciar o sistema (TL01).
- Dados de entrada: login, senha e papel (Mestre ou Jogador). O ID é gerado pelo sistema.

**Saídas e pós-condições**
- Sucesso: o usuário está guardado no armazenamento escolhido com um ID novo, gerado pelo sistema, e aparece na listagem.
- Falha: nada é alterado e o Visitante vê o motivo da recusa.

## Fluxo principal

1. Este caso de uso se inicia quando o Visitante escolhe "Cadastrar usuario" no menu principal (TL02).
2. O sistema pede login, senha e papel.
3. O Visitante informa os dados.
4. O sistema valida o login (RF02): não vazio, até 12 caracteres, sem números e ainda não usado [FE2].
5. O sistema valida a senha (RF03): de 8 a 128 caracteres, 3 de 4 tipos, diferente do login [FE3].
6. O sistema gera o próximo ID (sequencial, nunca reaproveitado) e guarda o usuário no armazenamento escolhido [FE4].
7. O sistema exibe "Usuario cadastrado com sucesso. ID: " seguido do ID gerado e volta ao menu.

## Fluxos secundários

**FE2 — Login inválido (passo 4)**
1. Caso o login viole uma regra, o sistema exibe "Login invalido:" seguido da regra violada (RF01, RF02), por exemplo "Login ja esta em uso.".
2. O caso de uso volta ao menu sem alterar nada.

**FE3 — Senha inválida (passo 5)**
1. Caso a senha viole uma regra, o sistema exibe "Senha invalida:" seguido da regra violada (RF03), sem repetir a senha digitada (NF-SEG-01).
2. O caso de uso volta ao menu sem alterar nada.

**FE4 — Falha ao gravar o arquivo binário (passo 6)**
1. Caso a gravação falhe, o sistema desfaz o cadastro em memória e exibe uma mensagem iniciada por "Falha no armazenamento:" (NF-CON-01).
2. O sistema continua em execução e volta ao menu.

**FA1 — Papel inválido (passo 3)**
1. Caso o Visitante digite algo diferente de 1 ou 2 no papel, o sistema exibe "Papel invalido." e volta ao menu.

> O antigo FE1 (ID inválido) deixou de existir com o ID automático (ADR-0003); o identificador não é reaproveitado.
