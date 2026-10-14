# rnf/confiabilidade Specification

## Purpose
Requisitos não funcionais de integridade dos dados gravados e de comportamento do sistema diante de falhas de armazenamento.

## Requirements

### Requirement: NF-CON-01 — Falha de armazenamento tratada
O sistema NÃO DEVE encerrar nem perder os usuários que já estão em memória quando a gravação ou a leitura do arquivo binário falhar; DEVE tratar a exceção e informar ao usuário que a operação não foi concluída.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC04

#### Scenario: Arquivo sem permissão de escrita
- **GIVEN** o sistema usa o arquivo binário, tem 3 usuários e o arquivo fica sem permissão de escrita
- **WHEN** o Visitante cadastra um usuário válido
- **THEN** o sistema exibe uma mensagem iniciada por "Falha no armazenamento:", continua em execução e a listagem mostra os mesmos 3 usuários de antes

### Requirement: NF-CON-02 — Arquivo corrompido detectado
O sistema DEVE detectar, ao iniciar, um arquivo binário de usuários truncado ou corrompido e informar o problema em vez de carregar dados parciais.

**Prioridade:** Importante
**Casos de uso:** UC03

#### Scenario: Arquivo truncado
- **GIVEN** o arquivo `usuarios.dat` teve os últimos bytes apagados
- **WHEN** o sistema é iniciado com o armazenamento em arquivo binário
- **THEN** o sistema exibe uma mensagem iniciada por "Falha no armazenamento:" e 0 usuários parciais são carregados

### Requirement: NF-CON-03 — Durabilidade dos cadastros
O sistema DEVE gravar no arquivo binário cada cadastro ou exclusão no momento em que a operação é confirmada.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC04

#### Scenario: Encerramento logo após o cadastro
- **GIVEN** o sistema usa o arquivo binário
- **WHEN** um usuário é cadastrado e o processo é encerrado logo em seguida, sem usar a opção "Sair"
- **THEN** 100% dos cadastros confirmados aparecem na listagem da próxima execução
