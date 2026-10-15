# rnf/manutenibilidade Specification

## Purpose
Requisitos não funcionais de facilidade de modificar e estender o sistema, como trocar o armazenamento ou acrescentar regras de validação.

## Requirements

### Requirement: NF-MAN-01 — Armazenamento substituível
O sistema DEVE isolar o armazenamento atrás de uma interface de repositório, de modo que trocar ou acrescentar um mecanismo (memória, arquivo binário, banco de dados) não exija alterar as classes de controle nem as de validação.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC03, UC04

#### Scenario: Novo mecanismo de armazenamento
- **GIVEN** o sistema já funciona com os armazenamentos em memória e em arquivo binário
- **WHEN** a equipe cria um terceiro mecanismo que implementa a interface de repositório
- **THEN** 0 arquivos das pastas de controle e de validação são alterados; só a criação do repositório no início da execução muda

### Requirement: NF-MAN-02 — Nova regra de validação sem alterar as existentes
O sistema DEVE permitir acrescentar uma regra de validação de login ou de senha criando uma nova classe de regra, sem modificar as regras já existentes.

**Prioridade:** Importante
**Casos de uso:** UC01

#### Scenario: Regra extra de senha
- **GIVEN** as 3 regras de senha atuais
- **WHEN** a equipe acrescenta uma regra "senha não pode conter o login"
- **THEN** 1 classe nova é criada, 1 linha de registro é acrescentada no controle e 0 classes de regra existentes são alteradas

### Requirement: NF-MAN-03 — Regras cobertas por testes automatizados
O sistema DEVE ter um teste automatizado para cada cenário dos requisitos RF02 e RF03, executável com um único comando.

**Prioridade:** Importante
**Casos de uso:** UC01

#### Scenario: Execução dos testes
- **GIVEN** o código compila sem erros
- **WHEN** a equipe executa `make test`
- **THEN** 100% dos cenários de RF02 e RF03 têm teste correspondente e todos passam em menos de 5 segundos
