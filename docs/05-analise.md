# Análise de casos de uso — diagrama de classes de análise

> Seção 7 do modelo. Classes **sem atributos e métodos**, mas com os relacionamentos já definidos.
> Estereótipos ICONIX: «boundary» (fronteira), «control» (controle), «entity» (entidade).
> Os nomes das entidades vêm do [glossário](02-glossario.md).

## 7.1 Gerenciamento de usuários (S1.2)

```mermaid
classDiagram
    direction LR

    class TelaArmazenamento { <<boundary>> }
    class TelaCriarConta { <<boundary>> }
    class TelaLogin { <<boundary>> }
    class TelaUsuarios { <<boundary>> }

    class ControleCriarConta { <<control>> }
    class ControleUsuarios { <<control>> }
    class ControleLogin { <<control>> }

    class Usuario { <<entity>> }
    class Papel { <<entity>> }
    class Armazenamento { <<entity>> }

    Usuario "*" --> "1" Papel
    Armazenamento "1" o-- "*" Usuario : guarda

    TelaArmazenamento ..> ControleCriarConta : escolhe armazenamento
    TelaCriarConta ..> ControleCriarConta
    TelaUsuarios ..> ControleUsuarios : listar, excluir
    TelaLogin ..> ControleLogin
    ControleCriarConta ..> Usuario
    ControleCriarConta ..> Armazenamento
    ControleUsuarios ..> Usuario
    ControleUsuarios ..> Armazenamento
    ControleLogin ..> Armazenamento
    ControleLogin ..> Usuario
```

### Rastreabilidade caso de uso → classes de controle

| Caso de uso | Controle | Fronteiras |
|---|---|---|
| UC01 Criar conta | ControleCriarConta | TelaArmazenamento, TelaCriarConta |
| UC02 Fazer login | ControleLogin | TelaLogin |
| UC03 Listar usuários | ControleUsuarios | TelaUsuarios |
| UC04 Excluir conta | ControleUsuarios | TelaUsuarios |

> Na implementação de console (Sprints 1 e 2), as fronteiras TelaArmazenamento, TelaCriarConta e TelaUsuarios são realizadas pela classe `MenuPrincipal` (ver [08-projeto.md](08-projeto.md)). ControleLogin e TelaLogin entram com a autenticação (RF07).

## 7.2 Sistema completo

O diagrama de análise do sistema inteiro (campanhas, personagens, rolagens e participações) foi feito na Sprint 1 e está em [`diagramas/DiagramaClasses-analise.pdf`](diagramas/DiagramaClasses-analise.pdf):

![Diagrama de classes de análise do sistema](diagramas/DiagramaClasses-analise.png)

Entidades principais e relacionamentos:

```mermaid
classDiagram
    direction LR
    class Usuario { <<entity>> }
    class Campanha { <<entity>> }
    class Personagem { <<entity>> }
    class Participacao { <<entity>> }
    class Rolagem { <<entity>> }
    class TipoDado { <<enumeration>> }

    Usuario "1" --> "*" Campanha : mestre de
    Usuario "1" --> "*" Personagem : jogador de
    Personagem "1" --> "*" Participacao
    Campanha "1" --> "*" Participacao
    Campanha "1" --> "*" Rolagem
    Personagem "1" --> "*" Rolagem
    Rolagem --> TipoDado
```
