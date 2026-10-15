# rnf/portabilidade Specification

## Purpose
Requisitos não funcionais sobre as plataformas em que o sistema compila e executa.

## Requirements

### Requirement: NF-POR-01 — Linux e Windows sem dependências externas
O sistema DEVE compilar com g++ (C++17) e executar em Linux e em Windows usando apenas a biblioteca padrão, sem bibliotecas de terceiros.

**Prioridade:** Essencial

#### Scenario: Compilação nas duas plataformas
- **GIVEN** um computador Linux com g++ 9 ou superior e um Windows com MinGW g++ 9 ou superior
- **WHEN** a equipe executa `make` em `src/` em cada um
- **THEN** a compilação termina com 0 erros e 0 avisos com `-Wall -Wextra`, nas 2 plataformas

### Requirement: NF-POR-02 — Arquivo binário portável entre as plataformas
O sistema DEVE gravar o arquivo binário com inteiros de tamanho fixo, de modo que um arquivo criado em uma plataforma seja lido na outra.

**Prioridade:** Desejável
**Casos de uso:** UC03

#### Scenario: Arquivo criado no Windows lido no Linux
- **GIVEN** um `usuarios.dat` com 3 usuários gravado no Windows (x86-64)
- **WHEN** o sistema é iniciado no Linux (x86-64) com esse arquivo
- **THEN** a listagem exibe os 3 usuários
