# [UC12] Editar ficha do personagem

Este caso de uso é responsável por atualizar a ficha de um personagem já criado (por exemplo, ao subir de nível), sem mudar o ID nem o dono.

| Campo | Valor |
|---|---|
| **Ator** | Usuário (Mestre ou Jogador) |
| **Prioridade** | Essencial |
| **Requisitos** | RF10, RF05, NF-USA-01, NF-USA-03, NF-CON-01 |
| **Interfaces** | TL03, TL05 |

**Entradas e pré-condições**
- O armazenamento já foi escolhido ao iniciar o sistema (TL01).
- Existe ao menos um personagem (UC07). O Usuário pode consultar os IDs na listagem (UC13).
- Dados de entrada: ID do personagem e a nova ficha (nome, nível, pontos de vida e os seis atributos).

**Saídas e pós-condições**
- Sucesso: o personagem guarda a nova ficha, com o mesmo ID e o mesmo dono.
- Falha: a ficha anterior continua guardada e o Usuário vê o motivo da recusa.

## Fluxo principal

1. Este caso de uso se inicia quando o Usuário escolhe "Personagens" no menu principal e depois "Editar ficha" (TL03).
2. O sistema pede o ID do personagem.
3. O Usuário informa o ID [FA1].
4. O sistema busca o personagem [FE1] e exibe a ficha atual, com os modificadores de cada atributo (TL05).
5. O sistema pede a nova ficha: nome, nível, pontos de vida e cada atributo.
6. O Usuário informa os dados [FA1].
7. O sistema valida a nova ficha com as mesmas regras de UC07 [FE2].
8. O sistema substitui a ficha, mantendo o ID e o dono, e grava no armazenamento escolhido [FE3].
9. O sistema exibe "Ficha atualizada com sucesso." e volta ao menu de personagens.

## Fluxos secundários

**FE1 — Personagem inexistente (passo 4)**
1. Caso não exista personagem com o ID informado, o sistema exibe "Personagem nao encontrado." e não pede a ficha.
2. O caso de uso volta ao menu de personagens.

**FE2 — Ficha inválida (passo 7)**
1. Caso algum campo viole uma regra, o sistema exibe "Ficha invalida:" seguido da regra violada (NF-USA-01).
2. O caso de uso volta ao menu de personagens; a ficha anterior não é alterada.

**FE3 — Falha ao gravar o arquivo binário (passo 8)**
1. Caso a gravação de `personagens.dat` falhe, o sistema volta à ficha anterior em memória e exibe uma mensagem iniciada por "Falha no armazenamento:" (NF-CON-01).
2. O sistema continua em execução e volta ao menu de personagens.

**FA1 — Valor não numérico (passos 3 e 6)**
1. Caso o Usuário digite algo que não é número, o sistema exibe "Valor invalido." e volta ao menu de personagens (NF-USA-03).
