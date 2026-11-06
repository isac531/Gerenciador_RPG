# [UC07] Criar personagem

Este caso de uso é responsável por criar a ficha de um personagem e ligá-la ao usuário dono (Usuário 1 → * Personagem).

| Campo | Valor |
|---|---|
| **Ator** | Usuário (Mestre ou Jogador) |
| **Prioridade** | Essencial |
| **Requisitos** | RF08, RF05, NF-USA-01, NF-USA-03, NF-CON-01 |
| **Interfaces** | TL03, TL04 |

**Entradas e pré-condições**
- O armazenamento já foi escolhido ao iniciar o sistema (TL01).
- O usuário dono já tem conta (UC01). Enquanto a autenticação (UC02) não existe, o ID do dono é informado no menu.
- Dados de entrada: ID do usuário dono, nome, nível, pontos de vida e os seis atributos (Força, Destreza, Constituição, Inteligência, Sabedoria e Carisma).

**Saídas e pós-condições**
- Sucesso: o personagem está guardado com um ID gerado pelo sistema, ligado ao dono, e aparece na listagem de personagens (UC13).
- Falha: nada é alterado e o Usuário vê o motivo da recusa.

## Fluxo principal

1. Este caso de uso se inicia quando o Usuário escolhe "Personagens" no menu principal e depois "Criar personagem" (TL03).
2. O sistema pede o ID do usuário dono.
3. O Usuário informa o ID [FA1].
4. O sistema pede a ficha: nome, nível (1 a 20), pontos de vida e cada atributo (1 a 30) (TL04).
5. O Usuário informa os dados [FA1].
6. O sistema verifica se existe usuário com o ID informado [FE1].
7. O sistema valida a ficha: nome de 1 a 30 caracteres, nível de 1 a 20, pontos de vida maiores que zero e cada atributo de 1 a 30 [FE2].
8. O sistema gera o próximo ID de personagem e guarda o personagem no armazenamento escolhido [FE3].
9. O sistema exibe "Personagem criado com sucesso. ID: " seguido do ID gerado e volta ao menu de personagens.

## Fluxos secundários

**FE1 — Dono inexistente (passo 6)**
1. Caso não exista usuário com o ID informado, o sistema exibe "Nao foi possivel criar: Nao existe usuario com esse ID para ser o dono.".
2. O caso de uso volta ao menu de personagens sem alterar nada.

**FE2 — Ficha inválida (passo 7)**
1. Caso algum campo viole uma regra, o sistema exibe "Ficha invalida:" seguido da regra violada, por exemplo "O nivel deve estar entre 1 e 20." ou "O atributo Carisma deve estar entre 1 e 30." (NF-USA-01).
2. O caso de uso volta ao menu de personagens sem alterar nada.

**FE3 — Falha ao gravar o arquivo binário (passo 8)**
1. Caso a gravação de `personagens.dat` falhe, o sistema desfaz a criação em memória e exibe uma mensagem iniciada por "Falha no armazenamento:" (NF-CON-01).
2. O sistema continua em execução e volta ao menu de personagens.

**FA1 — Valor não numérico (passos 3 e 5)**
1. Caso o Usuário digite algo que não é número no ID, no nível, nos pontos de vida ou em um atributo, o sistema exibe "Valor invalido." e volta ao menu de personagens (NF-USA-03).
