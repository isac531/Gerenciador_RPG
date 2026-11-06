## MODIFIED Requirements

### Requirement: RF05 — Escolher o armazenamento
O sistema DEVE perguntar, no início da execução, onde os usuários e os personagens serão armazenados: em memória (RAM) ou em arquivo binário (`usuarios.dat` e `personagens.dat`); no arquivo binário, os dados já gravados DEVEM ser carregados ao iniciar.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC03, UC04, UC07, UC12, UC13, UC14

#### Scenario: Armazenamento em memória
- **GIVEN** o sistema é iniciado com a opção "Memoria"
- **WHEN** 2 usuários são cadastrados, o sistema é fechado e iniciado de novo com a opção "Memoria"
- **THEN** a listagem exibe "Nenhum usuario cadastrado."

#### Scenario: Armazenamento em arquivo binário
- **GIVEN** o sistema é iniciado com a opção "Arquivo binario"
- **WHEN** 2 usuários e 1 personagem são cadastrados, o sistema é fechado e iniciado de novo com a mesma opção
- **THEN** a listagem exibe os mesmos 2 usuários e a listagem de personagens exibe o mesmo personagem

### Requirement: RF06 — Excluir usuário
O sistema DEVE permitir excluir um usuário pelo ID, excluindo também os personagens dele, e informar quando o ID não existe.

**Prioridade:** Importante
**Casos de uso:** UC04

#### Scenario: Exclusão de usuário existente
- **GIVEN** existe o usuário de ID 2
- **WHEN** o usuário pede a exclusão do ID 2
- **THEN** o sistema exibe "Usuario e seus personagens excluidos com sucesso." e o ID 2 some da listagem

#### Scenario: Exclusão em cascata dos personagens
- **GIVEN** o usuário de ID 2 tem 2 personagens e o usuário de ID 3 tem 1 personagem
- **WHEN** o usuário pede a exclusão do ID 2
- **THEN** os 2 personagens do usuário 2 somem da listagem de personagens e o personagem do usuário 3 continua

#### Scenario: ID inexistente
- **GIVEN** não existe usuário de ID 9
- **WHEN** o usuário pede a exclusão do ID 9
- **THEN** o sistema exibe "Usuario nao encontrado." e nada é alterado
