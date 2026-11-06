# Design: CRUD de personagens

## Entidade

`Personagem` tem `id`, `idDono` (o ID do usuário) e uma `Ficha` com nome, nível, pontos de vida e os seis atributos (`Atributos`). A ficha é um objeto de valor: agrupa os dados que a interface lê e que a edição substitui, e evita construtores com mais de três parâmetros (CONTRIBUTING §4.2). O modificador de cada atributo é calculado por `calcularModificador`, igual ao glossário.

| Campo | Regra | Exceção |
|---|---|---|
| Nome | 1 a 30 caracteres, não só espaços | NomePersonagemInvalidoException |
| Nível | 1 a 20 (SRD 5.1) | NivelInvalidoException |
| Pontos de vida | maior que zero | PontosDeVidaInvalidosException |
| Atributos | cada um de 1 a 30 (SRD 5.1) | AtributoInvalidoException |
| Dono | ID de usuário existente | IdInvalidoException |

As regras seguem o mesmo Strategy das regras de usuário: `IRegraFicha` e uma classe por regra, registradas em `ControlePersonagem`.

## Persistência

`IPersonagemRepositorio` segue o molde de `IUsuarioRepositorio`, com ID automático ([ADR-0003](../../../../docs/07-arquitetura/adr/0003-id-automatico.md)) e a operação `atualizar`. `RepositorioPersonagemArquivoBinario` herda de `RepositorioPersonagemMemoria` e grava `personagens.dat` a cada alteração, desfazendo a alteração em memória se a gravação falhar. As funções de leitura e escrita binária foram extraídas para `infra/ArquivoBinario`, usadas pelos dois repositórios em arquivo.

## Fachada única

`FacadeSingletonController` ([ADR-0004](../../../../docs/07-arquitetura/adr/0004-fachada-singleton.md)) é o único ponto de entrada da `ui` na camada de negócio. Ela cria e guarda os três controles (`ControleCriarConta`, `ControleUsuarios`, `ControlePersonagem`), repassa cada pedido e coordena a exclusão em cascata. `obterQuantidadeEntidades()` soma usuários e personagens.
