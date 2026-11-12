# ADR-0005: Repository por entidade e Factory Method para escolher o armazenamento

- **Status:** aceito
- **Data:** 2026-11-11
- **Requisitos relacionados:** RF05, NF-MAN-01, NF-CON-01

## Contexto

Até a Sprint 3, cada entidade tinha uma interface de repositório (`IUsuarioRepositorio`, `IPersonagemRepositorio`), mas a escolha da implementação ficava em funções livres da `infra` (`criarRepositorioUsuarios(tipo)`, `criarRepositorioPersonagens(tipo)`), cada uma com o seu `if` sobre `TipoArmazenamento`. Cada entidade nova repetia o `if`, e nada garantia que os repositórios de um mesmo mecanismo fossem criados juntos. A Sprint 4 pede o padrão **Repository** para todas as entidades e **Factory Method** (ou Abstract Factory) para criar as entidades ou escolher o tipo de repositório. O material da disciplina (Fowler, *PoEAA*; "O Padrão Repository na Clean Architecture") pede que a interface fique no domínio, só com tipos de domínio, e que a implementação fique na infraestrutura.

## Decisão

**Repository (Fowler).** Cada entidade persistida tem a sua interface em `business/repository`, que trata os objetos como uma coleção em memória e usa só tipos do domínio:

| Entidade | Interface (business) | Implementações (infra) |
|---|---|---|
| Usuário | `IUsuarioRepositorio` | `RepositorioMemoria`, `RepositorioArquivoBinario` (`usuarios.dat`) |
| Personagem | `IPersonagemRepositorio` | `RepositorioPersonagemMemoria`, `RepositorioPersonagemArquivoBinario` (`personagens.dat`) |
| Registro de acesso | `IAcessoRepositorio` (só `adicionar` e consultas: o histórico não se altera) | `RepositorioAcessoMemoria`, `RepositorioAcessoArquivoBinario` (`acessos.dat`) |

Os controles recebem as interfaces pelo construtor e não conhecem a `infra`. Regras de negócio (login repetido, dono existente, ficha válida) ficam nos controles, nunca no repositório. A versão em memória é também o *test double* dos testes de unidade.

**Factory Method (GoF).** `FabricaRepositorio` (classe abstrata em `business/repository`, papel de *Creator*) declara um *factory method* por entidade (`criarRepositorioUsuarios()`, `criarRepositorioPersonagens()`, `criarRepositorioAcessos()`) e a operação `criarRepositorios()`, que monta o conjunto completo sem conhecer as classes concretas. `FabricaMemoria` e `FabricaArquivoBinario` (na `infra`, *Creators* concretos) decidem qual classe instanciar. `escolherFabrica(tipo)` (`infra/EscolhaFabrica`) é o único lugar com `if` sobre `TipoArmazenamento`; o `main.cpp` chama `escolherFabrica(tipo)->criarRepositorios()`.

As funções livres antigas (`infra/FabricaRepositorio.h/.cpp`) foram removidas: depois da fábrica nova, nada mais as chamava.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| Manter as funções livres com `if` por entidade | Cada entidade nova repete o `if`; um novo mecanismo (ex.: SQLite) exige alterar todas as funções (viola OCP) |
| Abstract Factory com uma família de produtos por mecanismo | É o que a fábrica já faz na prática (um *Creator* por mecanismo, um produto por entidade); o nome Factory Method foi mantido porque cada operação `criar...()` é sobrescrita pelas subclasses, e o enunciado aceita os dois |
| Repositório genérico `Repositorio<T>` | O material da disciplina o aponta como anti-pattern: esconde a intenção do negócio e força filtros em memória nos controles |
| Um único repositório "Deus" para todas as entidades | Fere o ISP e dificulta os testes |

## Consequências

- Positivas: novo mecanismo de armazenamento = nova subclasse de `FabricaRepositorio`, sem tocar em controles, fachada ou testes de regra (NF-MAN-01). Nova entidade = um *factory method* a mais e uma implementação em cada fábrica concreta. Os testes de controle continuam usando os repositórios em memória.
- Negativas: cada entidade nova obriga a alterar todas as fábricas concretas (custo conhecido do padrão). Os repositórios em arquivo continuam regravando o arquivo inteiro a cada alteração, aceitável para o volume do projeto (NF-DES-01).
