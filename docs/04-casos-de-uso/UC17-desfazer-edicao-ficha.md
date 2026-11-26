# [UC17] Desfazer edição da ficha

Este caso de uso é responsável por voltar a ficha de um personagem ao estado anterior à última edição (UC12), por exemplo quando o Usuário digitou um nível ou pontos de vida errados. Estende o UC12: só faz sentido depois de uma edição. Implementado com o padrão Memento ([ADR-0009](../07-arquitetura/adr/0009-memento-desfazer-edicao.md)).

| Campo | Valor |
|---|---|
| **Ator** | Usuário (Mestre ou Jogador) |
| **Prioridade** | Essencial |
| **Requisitos** | RF15, RF10, RF05, NF-CON-01 |
| **Interfaces** | TL03 |

**Entradas e pré-condições**
- O armazenamento já foi escolhido ao iniciar o sistema (TL01).
- Para haver o que desfazer, uma ficha foi editada com sucesso (UC12) na execução atual.
- Não há dados de entrada: o sistema sabe qual foi a última edição.

**Saídas e pós-condições**
- Sucesso: o personagem da última edição volta à ficha anterior, com o mesmo ID e o mesmo dono, gravada no armazenamento escolhido. Não há mais edição para desfazer.
- Falha: o personagem continua com a ficha editada e o Usuário vê o motivo.

## Fluxo principal

1. Este caso de uso se inicia quando o Usuário escolhe "Personagens" no menu principal e depois "Desfazer ultima edicao de ficha" (TL03).
2. O sistema busca a última edição guardada [FA1].
3. O sistema volta o personagem dessa edição à ficha anterior e grava no armazenamento escolhido [FA2] [FE1].
4. O sistema exibe "Edicao desfeita. Ficha restaurada:" seguido da ficha do personagem, com os modificadores de cada atributo, e volta ao menu de personagens.

## Fluxos secundários

**FA1 — Nenhuma edição para desfazer (passo 2)**
1. Caso nenhuma ficha tenha sido editada na execução atual, ou a última edição já tenha sido desfeita, o sistema exibe "Nenhuma edicao para desfazer.".
2. O caso de uso volta ao menu de personagens; nada é alterado.

**FA2 — Personagem excluído depois da edição (passo 3)**
1. Caso o personagem da última edição tenha sido excluído (UC14 ou UC04), o sistema descarta a edição guardada e exibe "Nenhuma edicao para desfazer.".
2. Nenhum personagem é criado.

**FE1 — Falha ao gravar o arquivo binário (passo 3)**
1. Caso a gravação de `personagens.dat` falhe, o sistema mantém a ficha editada em memória e exibe uma mensagem iniciada por "Falha no armazenamento:" (NF-CON-01).
2. A edição continua guardada: o Usuário pode tentar desfazer de novo.

## Regras

- Só a última edição pode ser desfeita (RF15). Uma nova edição, de qualquer personagem, substitui a anterior.
- A edição guardada vale só para a execução atual: fechar o sistema a descarta.
