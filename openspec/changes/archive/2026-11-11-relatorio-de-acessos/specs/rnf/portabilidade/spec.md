## MODIFIED Requirements

### Requirement: NF-POR-01 — Linux e Windows sem dependências externas
O sistema DEVE compilar com g++ (C++17) e executar em Linux e em Windows sem exigir a instalação de nenhuma biblioteca. Bibliotecas de terceiros só são aceitas se tiverem licença livre, forem copiadas em código-fonte para `src/infra/externo/` e compilarem com o mesmo `make`.

**Prioridade:** Essencial

#### Scenario: Compilação nas duas plataformas
- **GIVEN** um computador Linux com g++ e gcc 9 ou superior e um Windows com MinGW 9 ou superior
- **WHEN** a equipe executa `make` em `src/` em cada um, sem instalar nada além do compilador
- **THEN** a compilação termina com 0 erros e 0 avisos com `-Wall -Wextra`, nas 2 plataformas
