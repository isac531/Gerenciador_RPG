# ADR-0009: Memento para desfazer a última edição da ficha

- **Status:** aceito
- **Data:** 2026-11-26
- **Requisitos relacionados:** RF15, RF10, NF-CON-01, NF-MAN-01

## Contexto

A Sprint 5 (S5.2) pede o padrão **Memento** para desfazer a última atualização da entidade principal. A entidade principal é o **Personagem** (Sprint 3), e a única atualização dele é a edição da ficha (RF10, UC12). O `Personagem` é imutável e o repositório só guarda o estado atual, então, depois de uma edição, a ficha anterior se perde. Desde o [ADR-0008](0008-fachada-command.md) cada caso de uso é um comando executado pela fachada, e o desfazer precisa seguir o mesmo caminho.

## Decisão

Usar o Memento com os três papéis clássicos, ligado ao Command da S5.1:

| Papel | Classe | O que faz |
|---|---|---|
| Originator | `Personagem` (`business/model`) | `criarMemento()` guarda a ficha atual; `restaurar(memento)` devolve uma cópia com a ficha guardada, com o mesmo ID e o mesmo dono. Lança `IdInvalidoException` se o memento for de outro personagem |
| Memento | `MementoPersonagem` (`business/memento`) | Guarda o ID do personagem e a ficha. O construtor e a ficha são privados e só o `Personagem` os acessa (`friend`): para o resto do sistema o memento é opaco e só informa o ID |
| Caretaker | `HistoricoEdicaoPersonagem` (`business/memento`) | Guarda **um único** memento, o da última edição (`std::optional`). Guardar outro substitui o anterior; desfazer o descarta. Nunca lê a ficha |

Ligação com o Command:

- `ControlePersonagem::editarFicha()` passa a devolver o memento do estado **anterior**, e `ControlePersonagem::restaurar(memento)` volta o personagem a ele e grava no repositório, sem validar a ficha de novo (ela já tinha sido aceita).
- `EditarPersonagemComando` guarda o memento no histórico **depois** de a edição ser aceita e gravada. Edição recusada não mexe no histórico.
- Novo `DesfazerEdicaoPersonagemComando` (*ConcreteCommand* do UC17) lê o memento, pede a restauração e descarta o memento. Se a gravação falhar, o histórico fica como estava, para uma nova tentativa (NF-CON-01). Se o personagem foi excluído, nada é restaurado e o histórico é descartado. Como os IDs não são reaproveitados ([ADR-0003](0003-id-automatico.md)), um memento nunca cai em outro personagem.
- A `FacadeSingletonController` é dona do histórico (é a única instância, então o histórico também é único), o passa aos dois comandos e ganha `desfazerUltimaEdicao()`, que devolve o personagem restaurado ou `std::nullopt`. `inicializar()` esvazia o histórico.
- O histórico vive só na memória da execução atual, mesmo com armazenamento em arquivo binário.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| `desfazer()` no próprio `EditarPersonagemComando`, guardando a ficha anterior num campo | Os comandos têm vida curta e são criados dentro do método da fachada (ADR-0008); seria preciso guardar o comando inteiro, com referências a controles e ao logger, além da chamada. E o comando passaria a conhecer o estado interno do personagem, que é o que o Memento evita |
| Caretaker com pilha de mementos | O enunciado pede só a última atualização; uma pilha traz perguntas que ninguém pediu (limite, refazer, edições de personagens diferentes) |
| Memento com getters públicos (uma `struct` com a ficha) | Funciona, mas qualquer classe poderia ler e alterar o estado guardado, quebrando o encapsulamento que o padrão protege |
| Guardar a versão anterior no repositório (coluna "ficha anterior" em `personagens.dat`) | Muda o formato do arquivo e a interface `IPersonagemRepositorio` por uma necessidade de sessão; o desfazer não precisa sobreviver ao fechamento do sistema |

## Consequências

- Positivas: o estado anterior fica encapsulado (só o `Personagem` o lê); a fachada e a `ui` não sabem o que há na ficha guardada; o desfazer segue o mesmo caminho dos outros casos de uso (comando + invocador, log e tratamento de falha de armazenamento); acrescentar desfazer para outra entidade é repetir o par Originator/Memento e um comando.
- Negativas / riscos: só uma edição pode ser desfeita, em qualquer personagem: editar o personagem 2 depois do 1 faz perder o desfazer do 1. O histórico é global, não por usuário, porque ainda não há sessão (RF07 parcial). Fechar o programa perde o histórico.
