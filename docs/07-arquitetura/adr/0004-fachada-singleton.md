# ADR-0004: Fachada única com Singleton para os controles

- **Status:** aceito
- **Data:** 2026-11-06
- **Requisitos relacionados:** RF08–RF12, NF-MAN-01

## Contexto

Com o CRUD de personagens, a camada de negócio passou a ter três controles (`ControleCriarConta`, `ControleUsuarios`, `ControlePersonagem`). Sem uma fachada, a `ui` teria de conhecer e receber os três, e regras que envolvem mais de um controle (excluir um usuário e os personagens dele) ficariam no menu. A Sprint 3 pede uma fachada única (Facade + Singleton) com um método que retorna a quantidade de entidades cadastradas.

## Decisão

Criar `FacadeSingletonController` em `business/control`:

- **Facade:** é o único ponto de entrada da `ui` na camada de negócio. Repassa cada pedido ao controle do caso de uso e coordena as operações entre controles (`excluirUsuario` exclui primeiro os personagens do usuário). `obterQuantidadeEntidades()` soma usuários e personagens; `obterQuantidadeUsuarios()` e `obterQuantidadePersonagens()` dão o detalhe.
- **Singleton:** construtor privado, cópia proibida e `getInstancia()` com variável `static` local (inicialização thread-safe desde o C++11). Os repositórios continuam injetados: `main.cpp` chama `inicializar(usuarios, personagens)` uma vez, e a fachada cria os controles com eles (o Singleton não esconde as dependências concretas, preservando o [ADR-0001](0001-persistencia-substituivel.md)).
- `MenuPrincipal` e `MenuPersonagens` recebem a fachada por referência no construtor, em vez de chamar `getInstancia()` em todo lugar.

No diagrama de classes de análise a classe aparece como `FachadaControladores`; no código ela recebe o nome pedido no enunciado da sprint.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| A `ui` receber os três controles | Acopla a interface a todos os controles; cada controle novo muda o construtor do menu |
| Singleton que cria os repositórios por conta própria | Esconde a escolha do armazenamento (RF05) dentro do negócio e impede testar com repositórios em memória |
| Variável global | Sem controle de criação nem garantia de instância única |

## Consequências

- Positivas: a `ui` depende só da fachada; regras entre entidades ficam no negócio; testes reinicializam a fachada com repositórios novos em memória.
- Negativas: a fachada cresce a cada caso de uso novo; na Sprint 5 ela passa a usar Command, o que reduz esse acoplamento. Usar a fachada antes de `inicializar()` lança `std::logic_error` (erro de programação).
