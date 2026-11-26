# ADR-0008: Fachada que executa os casos de uso como comandos (Command)

- **Status:** aceito
- **Data:** 2026-11-24
- **Requisitos relacionados:** RF01, RF04, RF06–RF14, NF-MAN-01, NF-SEG-01

## Contexto

O [ADR-0004](0004-fachada-singleton.md) criou a `FacadeSingletonController` e já previa, nas consequências negativas, que ela cresceria a cada caso de uso novo. Na Sprint 4 isso se confirmou: cada método da fachada chamava o controle, registrava o resultado no log e tratava a recusa do usuário com `try/catch`, e um modelo de função (`registrandoFalhaDeArmazenamento`) repetia o tratamento de `FalhaArmazenamentoException` em cada operação. Um caso de uso novo obrigava a alterar a fachada em três pontos (declaração, lógica e log). A Sprint 5 (S5.1) pede que a fachada da camada de negócio passe a usar o padrão **Command**.

## Decisão

Cada caso de uso passa a ser um objeto-comando em `business/command`. A fachada continua sendo o único ponto de entrada da `ui` (Facade) e continua sendo uma instância única (Singleton); só muda o que acontece dentro de cada método dela.

- **Command:** `IComando` declara `executar()` e `descricaoFalha()` (texto do log quando o armazenamento falha). `ComandoComResultado<Resultado>` é a base dos comandos que devolvem valor: guarda o resultado depois de `executar()` e o entrega em `resultado()`.
- **ConcreteCommand:** um por caso de uso. Cada um guarda os parâmetros do pedido, recebe os controles e o `ILogger` por referência e contém o que a fachada fazia: chamar o controle, coordenar mais de um controle e registrar o evento no log (nunca a senha, NF-SEG-01).

  | Comando | Caso de uso | Receivers | Resultado |
  |---|---|---|---|
  | `CriarContaComando` | UC01 | `ControleCriarConta` | ID gerado |
  | `FazerLoginComando` | UC02 | `ControleLogin` | `Usuario` autenticado |
  | `ListarUsuariosComando` | UC03 | `ControleUsuarios` | lista de usuários |
  | `ExcluirUsuarioComando` | UC04 | `ControlePersonagem`, `ControleUsuarios` | `bool` (existia) |
  | `CriarPersonagemComando` | UC07 | `ControlePersonagem` | ID gerado |
  | `EditarPersonagemComando` | UC12 | `ControlePersonagem` | nenhum (herda de `IComando`) |
  | `ListarPersonagensComando` | UC13 | `ControlePersonagem` | lista de personagens |
  | `ExcluirPersonagemComando` | UC14 | `ControlePersonagem` | `bool` (existia) |
  | `BuscarPersonagemComando` | consulta usada pelo UC12 | `ControlePersonagem` | `optional<Personagem>` |
  | `ContarEntidadesComando` | UC15 | `ControleUsuarios`, `ControlePersonagem` | `QuantidadeEntidades` |
  | `GerarRelatorioAcessosComando` | UC16 | `ControleRelatorioAcessos` | `RelatorioGerado` |
  | `DesfazerEdicaoPersonagemComando` (S5.2, [ADR-0009](0009-memento-desfazer-edicao.md)) | UC17 | `ControlePersonagem` | `optional<Personagem>` restaurado |

- **Invoker:** `InvocadorComandos::executar(comando, logger)` executa qualquer `IComando` sem conhecer o caso de uso. É o único lugar que trata `FalhaArmazenamentoException`: registra `descricaoFalha()` + motivo no log e relança. As exceções de negócio (login, senha e ficha inválidos, ID inválido) passam sem alteração e continuam sendo tratadas na `ui`.
- **Client:** a `FacadeSingletonController` monta o comando com os controles que já possui, entrega-o ao invocador e devolve `comando.resultado()`. Cada método público ficou com duas ou três linhas.
- **Contrato público da fachada não muda:** os mesmos métodos, parâmetros, retornos e exceções. `MenuPrincipal`, `MenuPersonagens` e `main.cpp` não foram alterados, e os 73 testes anteriores passam sem edição.
- Regra para o futuro: caso de uso novo = comando novo em `business/command` + método novo na fachada que o monta. Foi o que aconteceu no desfazer da S5.2: `DesfazerEdicaoPersonagemComando` + `desfazerUltimaEdicao()`, e o `EditarPersonagemComando` passou a receber o histórico do Memento.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| A `ui` criar e executar os comandos | A `ui` passaria a conhecer controles e comandos, contrariando o [ADR-0004](0004-fachada-singleton.md) (a `ui` fala só com a fachada) |
| Comandos como `std::function` ou lambdas dentro da fachada | Menos classes, mas o comando não tem nome nem descrição de falha, não pode ser testado sem a fachada e não serve de base para guardar histórico (Memento, S5.2) |
| Manter os métodos com `try/catch` e o modelo `registrandoFalhaDeArmazenamento` | A fachada continua concentrando lógica e log de todos os casos de uso e cresce a cada um novo |
| Já incluir desfazer, fila ou histórico de comandos | Não é pedido nesta tarefa; o desfazer da atualização é o Memento da S5.2, e o `IComando` não impede acrescentar `desfazer()` depois |

## Consequências

- Positivas: a fachada fica fina e deixa de ter lógica de caso de uso; cada caso de uso fica numa classe pequena, testável sem a fachada (`test/testes_comando.cpp`); o tratamento de falha de armazenamento fica num ponto só; um caso de uso novo acrescenta classes sem alterar os comandos existentes; o `IComando` é o ponto de extensão para fila, histórico e desfazer.
- Negativas / riscos: 11 comandos, uma base e um invocador a mais para manter; os parâmetros são copiados para dentro do comando e o resultado é copiado na devolução. Os comandos guardam referências a controles e ao logger, por isso têm vida curta: são criados e executados dentro do método da fachada, nunca guardados além de uma chamada a `inicializar()`. O invocador passou a registrar a falha de armazenamento também nas consultas (antes só as operações que gravam eram registradas), o que não muda o comportamento visível ao usuário.
