# Proposta: desfazer a última atualização do personagem (Sprint 5)

## Por quê

A Sprint 5 pede duas mudanças de projeto: a fachada da camada de negócio passa a usar o padrão **Command** (S5.1) e o padrão **Memento** é usado para desfazer a última atualização da entidade principal (S5.2). A entidade principal do sistema é o **Personagem** (Sprint 3), e a única atualização dele é a edição da ficha (RF10, UC12). Hoje uma edição errada (por exemplo, um nível digitado a mais) só se corrige digitando a ficha inteira de novo.

## O que muda

- **ADDED** RF15 Desfazer a última edição da ficha: o usuário pode voltar a ficha do personagem ao estado anterior à última edição feita na execução atual. Só a última edição pode ser desfeita.
- Casos de uso: novo UC17 Desfazer edição da ficha, que estende o UC12 Editar ficha.
- Código: `MementoPersonagem` (Memento), `Personagem::criarMemento()` e `Personagem::restaurar()` (Originator), `HistoricoEdicaoPersonagem` (Caretaker) e o comando `DesfazerEdicaoPersonagemComando`, executado pela fachada como os demais comandos da S5.1. `EditarPersonagemComando` passa a guardar o memento do estado anterior. O menu de personagens ganha a opção "5. Desfazer ultima edicao de ficha".

## Fora do escopo

- Desfazer mais de uma edição (pilha de histórico) e refazer (*redo*).
- Desfazer criação ou exclusão de personagem, ou qualquer operação de usuário.
- Guardar o histórico entre execuções: o memento vive só na memória da execução atual, mesmo com o armazenamento em arquivo binário.
- Histórico por usuário autenticado: o sistema ainda não tem sessão (ver RF07, parcial).

## Impacto

- Specs: capacidade `personagens` (RF15 novo).
- Documentos: casos de uso (UC17 e diagrama completo), glossário (Memento), `07-arquitetura/`, `08-projeto.md` (diagrama da S5.3, tabela de padrões, rastreabilidade) e o ADR-0009.
