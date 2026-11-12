## MODIFIED Requirements

### Requirement: RF05 — Escolher o armazenamento
O sistema DEVE perguntar, no início da execução, onde os usuários, os personagens e os registros de acesso serão armazenados: em memória (RAM) ou em arquivo binário (`usuarios.dat`, `personagens.dat` e `acessos.dat`); no arquivo binário, os dados já gravados DEVEM ser carregados ao iniciar.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC02, UC03, UC04, UC07, UC12, UC13, UC14, UC16

#### Scenario: Armazenamento em memória
- **GIVEN** o sistema é iniciado com a opção "Memoria"
- **WHEN** 2 usuários são cadastrados, o sistema é fechado e iniciado de novo com a opção "Memoria"
- **THEN** a listagem exibe "Nenhum usuario cadastrado."

#### Scenario: Armazenamento em arquivo binário
- **GIVEN** o sistema é iniciado com a opção "Arquivo binario"
- **WHEN** 2 usuários e 1 personagem são cadastrados, 1 login é feito, o sistema é fechado e iniciado de novo com a mesma opção
- **THEN** a listagem exibe os mesmos 2 usuários, a listagem de personagens exibe o mesmo personagem e o relatório de acessos mostra 1 tentativa
