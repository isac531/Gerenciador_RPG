# Guia de Contribuição

Este documento define as práticas e convenções adotadas no **Gerenciador de RPG** (C++17, console). Todo membro da equipe — humano ou agente IA — deve segui-las.

**Resumo rápido:** parta de `develop`, crie uma branch `<tipo>/<escopo>-<descricao>`, faça commits pequenos em Conventional Commits (português, imperativo), rode `make` e `make test` em `src/`, abra um PR de até ~400 linhas para `develop` e peça a revisão de outro integrante. O merge é feito com **merge commit**.

---

## 1. Estratégia de branches principais

O repositório possui duas branches de longa duração. Nenhuma delas recebe commits diretos.

```
main ──────────────────────────────────────────────► produção
  ▲                                          ▲
  │  merge (release ou hotfix)               │ hotfix
  │                                          │
develop ──────────────────────────────────► merge antes da release
  ▲         ▲          ▲
  │         │          │
feat/x   fix/y     refactor/z   ← branches de trabalho (curta duração)
```

### `main`

- Representa o estado **atual de produção**
- Todo commit em `main` é uma versão entregável e estável
- Só recebe merges vindos de `develop` (via release) ou de `hotfix/*`
- Cada merge em `main` deve ser **tagueado** com a versão: `v1.2.0`

### `develop`

- Branch de **integração contínua** — onde o trabalho da equipe converge
- Deve sempre estar em estado funcional (CI passando)
- Branches de feature, fix e refactor partem daqui e retornam aqui via PR
- Periodicamente promovida para `main` quando um conjunto de funcionalidades está pronto para release

### Fluxo resumido

```
1. Partir de develop
   git checkout develop && git pull
   git checkout -b feat/personagem-entidade

2. Desenvolver com commits atômicos

3. Abrir PR de feat/personagem-entidade → develop
   (revisão de outro integrante, CI verde)

4. Merge em develop com merge commit (decisão do time: preserva os
   commits atômicos e o histórico de quem fez o quê)

5. Em cada data de entrega (docs/00-entregas.md):
   PR develop → main  +  tag da versão (v0.1.0, v0.2.0, ...)
```

### Hotfix (bug crítico em produção)

```
main → hotfix/99-descricao → PR para main  +  PR para develop
```

Um hotfix parte de `main`, é corrigido, e depois é integrado em `develop` também — para não perder a correção na próxima release.

---

## 2. Commits

### Formato (Conventional Commits)

```
<tipo>(<escopo>): <descrição curta no imperativo>

[corpo opcional — o quê e por quê, não como]

[rodapé opcional: refs, breaking changes]
```

**Tipos permitidos:**

| Tipo       | Quando usar                                               |
|------------|-----------------------------------------------------------|
| `feat`     | Nova funcionalidade visível ao usuário/sistema            |
| `fix`      | Correção de bug                                           |
| `refactor` | Mudança interna sem alterar comportamento externo         |
| `test`     | Adição ou correção de testes                              |
| `docs`     | Apenas documentação                                       |
| `chore`    | Build, CI, dependências, configs — sem código de produção |
| `style`    | Formatação, espaçamento — sem mudança de lógica           |
| `perf`     | Melhoria de performance                                   |
| `revert`   | Reverte um commit anterior                                |

**Escopo:** a entidade ou a camada afetada. Escopos usados no projeto:

| Escopo | Quando usar |
|---|---|
| `usuarios`, `personagem` | Mudança que atravessa camadas de uma entidade |
| `model`, `control`, `validation`, `exception`, `repository` | Pastas de `src/business/` |
| `infra`, `ui` | Pastas `src/infra/` e `src/ui/` |
| `test` | Só testes |
| `specs`, `casos-de-uso`, `projeto`, `adr`, `diagramas` | Documentação |
| `ci` | GitHub Actions, Makefile |

**Exemplos do próprio repositório:**

```
feat(usuarios): gerar ID do usuario automaticamente

feat(control): adicionar FacadeSingletonController e ControlePersonagem

refactor(infra): extrair leitura e gravacao binaria para ArquivoBinario

docs(adr): registrar fachada unica com Singleton (ADR-0004)
```

**Regras:**
- Descrição em **português**, no **imperativo** ("adicionar", não "adicionei" ou "adicionando")
- Máximo de **72 caracteres** na primeira linha
- Um commit = uma responsabilidade. Se precisar de "e" na descrição, considere dividir
- Commits de trabalho em progresso devem usar `wip:` e ser squashados antes do merge

---

## 3. Branches

```
<tipo>/<escopo-ou-issue>-<descricao-curta>
```

```
feat/usuarios-id-automatico
feat/personagem-entidade
refactor/infra-arquivo-binario
docs/entrega-2-sprint-3
```

**Regras:**
- Nunca commitar diretamente em `main` ou `develop`
- Branches de feature e fix partem de `develop` (ver seção 1)
- Hotfixes partem de `main` e são integrados em `main` e `develop`
- Delete a branch após o merge

---

## 4. Práticas de Clean Code

> Independentes de linguagem. Válidas para qualquer camada.

### 4.1 Nomes revelam intenção

```
// ❌
int n;
bool flag;
void proc(std::vector<int> v);

// ✅
int pontosDeVida;
bool loginEstaEmUso;
void excluirDoDono(int idDono);
```

- Nomes de **variáveis e funções**: descrevem *o que são* ou *o que fazem*
- Nomes de **classes**: substantivos (`Personagem`, `ControlePersonagem`)
- Nomes de **funções/métodos**: verbos (`calcular`, `validar`, `buscar`)
- Evite abreviações que exijam contexto mental (`ctrlPers` → `controlePersonagem`)
- Evite prefixos redundantes (`personagem.personagemId` → `personagem.getId()`)
- Use os termos do [glossário](docs/02-glossario.md) (`login`, não `username`; `Ficha`, não `sheet`)

### 4.2 Funções fazem uma coisa

- Uma função cabe em uma tela sem scroll → suspeite se precisar de mais
- Se você precisar usar "e" para descrever o que ela faz, extraia
- Parâmetros: idealmente 0–2; 3 é o limite; acima disso, agrupe em objeto

### 4.3 Não deixe comentários onde o código pode falar

```
// ❌ Comentário que repete o código
// Verifica se o nivel esta entre 1 e 20
if (ficha.nivel >= 1 && ficha.nivel <= 20) { ... }

// ✅ Nome que dispensa o comentário
if (estaEntre(ficha.nivel, NIVEL_MINIMO, NIVEL_MAXIMO)) { ... }
```

Comentários válidos: decisões de negócio não óbvias, workarounds com link para issue, advertências sobre efeitos colaterais.

### 4.4 Sem números mágicos ou strings soltas

```
// ❌
if (login.size() > 12) { ... }
papel = 1;

// ✅
constexpr std::size_t TAMANHO_MAXIMO_LOGIN = 12;
if (login.size() > TAMANHO_MAXIMO_LOGIN) { ... }
papel = Papel::MESTRE;
```

### 4.5 Regra do Escoteiro

> Deixe o código *ligeiramente* melhor do que você encontrou.

Não é refactoring massivo — é renomear uma variável confusa, extrair um bloco repetido, remover um TODO resolvido.

### 4.6 Testes são cidadãos de primeira classe

- Todo bug corrigido ganha um teste que o reproduz antes do fix
- Nomes de teste descrevem o cenário: `deve_recusar_login_com_numeros`
- Um teste = um comportamento verificado
- Testes não devem depender de ordem de execução: cada teste cria os próprios repositórios em memória (e reinicializa a fachada com eles)
- Testes ficam em `test/`, um arquivo por área (`testes.cpp`, `testes_personagem.cpp`, `testes_fachada.cpp`), e rodam com `cd src && make test`

---

## 5. Padrões de código C++ do projeto

### 5.1 Nomes e arquivos

| Elemento | Convenção | Exemplo |
|---|---|---|
| Classe, struct, enum | PascalCase | `ControlePersonagem`, `Ficha`, `Papel` |
| Interface (classe só com métodos virtuais puros) | prefixo `I` | `IPersonagemRepositorio`, `IRegraFicha` |
| Método e função | camelCase, verbo | `criarConta`, `obterQuantidadeEntidades` |
| Atributo privado | camelCase com `_` no final | `repositorio_`, `proximoId_` |
| Constante | `constexpr` em MAIÚSCULAS | `NIVEL_MAXIMO`, `SEM_ID` |
| Valores de enum | MAIÚSCULAS | `Papel::JOGADOR` |
| Exceção | sufixo `Exception`, derivada de `std::runtime_error` | `NivelInvalidoException` |
| Arquivo | um par `.h`/`.cpp` por classe, com o nome da classe | `ControlePersonagem.h` |

- Cabeçalhos com `#pragma once`; inclua só o que usa; nunca `using namespace std` em `.h`.
- Código e mensagens ao usuário em português, sem acentos nas mensagens de console (portabilidade do terminal do Windows).
- Compile sem avisos com `-Wall -Wextra` (`make`).

### 5.2 Camadas e dependências

- `ui` → `business` ← `infra` (ver [`docs/08-projeto.md`](docs/08-projeto.md)). A `business` não inclui nada de `ui` nem de `infra`.
- A `ui` fala **só** com a `FacadeSingletonController` ([ADR-0004](docs/07-arquitetura/adr/0004-fachada-singleton.md)). Caso de uso novo = método novo na fachada que delega a um controle.
- Os controles recebem as interfaces de repositório no construtor; só `main.cpp` conhece as classes concretas de `infra`.
- IDs de entidades são gerados pelo repositório e nunca reaproveitados ([ADR-0003](docs/07-arquitetura/adr/0003-id-automatico.md)).
- Regra de validação nova = nova classe que implementa `IRegraValidacao` ou `IRegraFicha`, registrada no construtor do controle (Strategy).
- Erros de dado do usuário viram exceções específicas, tratadas na `ui` com mensagem que diz a regra violada; erros de E/S viram `FalhaArmazenamentoException`.

### 5.3 Fluxo de requisitos (OpenSpec)

1. Requisito novo ou alterado começa por uma change em `openspec/changes/<nome>/` (proposal, design, tasks e o delta das specs com `ADDED`/`MODIFIED`).
2. `openspec validate --all` deve passar (o CI roda a mesma validação).
3. Ao terminar a implementação: `openspec archive <nome> -y`, no mesmo PR ou no seguinte.
4. IDs de requisitos (`RFnn`, `NF-CAT-nn`), casos de uso (`UCnn`) e telas (`TLnn`) nunca são renumerados nem reaproveitados.

---

## 6. Pull Requests e revisão de código

**Título:** mesmo formato de commit (`feat(auth): adicionar login com Google`)

**Checklist antes de abrir o PR:**
- [ ] `cd src && make` sem avisos e `make test` passando localmente
- [ ] Cobertura não regrediu
- [ ] Nenhum `TODO` novo sem issue associada
- [ ] Documentação atualizada (se aplicável)
- [ ] ADR criado (se decisão arquitetural foi tomada)

**Tamanho:** PRs grandes são difíceis de revisar. Prefira PRs focados. Se ultrapassar ~400 linhas, justifique ou divida. PRs que dependem de outro dizem no corpo qual vem antes.

**Revisão:**
- Todo PR para `develop` é revisado por pelo menos um integrante que não é o autor.
- O revisor confere: o requisito e o caso de uso citados no PR, nomes, SOLID, camadas, testes do cenário e mensagens ao usuário.
- Comentários são respondidos no próprio PR; a correção vira um novo commit (sem reescrever o histórico de uma branch já revisada).
- O merge é feito com **merge commit** depois da aprovação e do CI verde; a branch é apagada em seguida.

---

## 7. Para Agentes IA

Ao atuar neste repositório, siga adicionalmente:

1. **Leia `openspec/specs/` (e a change aberta em `openspec/changes/`) antes de implementar** — ali está o contrato do que deve ser feito
2. **Leia o ADR relevante** antes de propor mudanças arquiteturais
3. **Nunca altere contratos públicos** (interfaces, APIs, eventos) sem registrar um novo ADR
4. **Prefira edições cirúrgicas** a reescritas completas de arquivos existentes
5. **Documente suposições** no corpo do commit quando o requisito for ambíguo
6. **Não invente dependências** externas sem registrar no PR por que são necessárias
