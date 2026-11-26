# Design: desfazer a última atualização do personagem

## Papéis do Memento

| Papel | Classe | Responsabilidade |
|---|---|---|
| Originator | `Personagem` | `criarMemento()` fotografa a ficha; `restaurar(memento)` devolve uma cópia com a ficha guardada (o personagem é imutável) |
| Memento | `MementoPersonagem` | Guarda o ID do personagem e a ficha anterior. Só o `Personagem` lê a ficha (amizade em C++); para os demais o memento é opaco, só expõe o ID |
| Caretaker | `HistoricoEdicaoPersonagem` | Guarda **um** memento, o da última edição. Uma nova edição substitui o anterior; desfazer o descarta |

## Fluxo

1. `EditarPersonagemComando::executar()` chama `ControlePersonagem::editarFicha()`, que agora devolve o memento do estado **anterior** à edição. Só depois de a edição ter sido aceita e gravada o comando entrega o memento ao histórico; uma edição recusada (ficha inválida, ID inexistente, falha de armazenamento) não mexe no histórico.
2. `DesfazerEdicaoPersonagemComando::executar()` lê o memento do histórico e pede a `ControlePersonagem::restaurar()` que volte o personagem àquele estado e grave no repositório. A ficha restaurada não é validada de novo: ela já tinha sido aceita.
3. Se o personagem foi excluído depois da edição, não há o que restaurar: o histórico é descartado e o resultado é "nada a desfazer". Os IDs nunca são reaproveitados ([ADR-0003](../../../docs/07-arquitetura/adr/0003-id-automatico.md)), então o memento nunca cai em outro personagem.
4. Se a gravação falhar ao desfazer, o repositório volta ao estado anterior (NF-CON-01) e o memento **continua** no histórico, para o usuário tentar de novo.

## Onde fica o histórico

A fachada (Singleton) é dona do `HistoricoEdicaoPersonagem` e o passa aos dois comandos. `inicializar()` esvazia o histórico. O histórico não é persistido (ver "Fora do escopo").

Decisão registrada em [ADR-0009](../../../docs/07-arquitetura/adr/0009-memento-desfazer-edicao.md).
