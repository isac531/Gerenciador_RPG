## MODIFIED Requirements

### Requirement: RF01 — Cadastrar usuário
O sistema DEVE permitir que um Visitante crie uma conta informando login, senha e papel (Mestre ou Jogador). O sistema DEVE gerar o ID do usuário automaticamente, de forma sequencial a partir de 1, sem reaproveitar IDs de usuários excluídos, exibir o ID gerado, recusar logins já existentes e informar o motivo de qualquer recusa.

**Prioridade:** Essencial
**Casos de uso:** UC01

#### Scenario: Cadastro válido
- **GIVEN** não existe usuário com login "isac" e nenhum usuário foi cadastrado ainda
- **WHEN** o Visitante cadastra login "isac", senha "Dado#2026" e papel Mestre
- **THEN** o usuário é armazenado com ID 1, o sistema exibe "Usuario cadastrado com sucesso. ID: 1" e ele passa a aparecer na listagem

#### Scenario: ID já existente
- **GIVEN** já existe um usuário com ID 1
- **WHEN** o Visitante cadastra outro usuário válido
- **THEN** o sistema não repete o ID 1: o novo usuário recebe o ID 2, sem que o Visitante precise informar um número

#### Scenario: ID não positivo
- **GIVEN** nenhum usuário foi cadastrado ainda
- **WHEN** o Visitante cadastra o primeiro usuário
- **THEN** o sistema gera o ID 1; o Visitante não informa o ID, então nunca há ID zero ou negativo

#### Scenario: ID de usuário excluído não é reaproveitado
- **GIVEN** foram cadastrados os usuários de ID 1 e 2 e o usuário de ID 2 foi excluído
- **WHEN** o Visitante cadastra outro usuário válido, mesmo depois de reiniciar o sistema com o arquivo binário
- **THEN** o novo usuário recebe o ID 3

#### Scenario: Login já existente
- **GIVEN** já existe um usuário com login "isac"
- **WHEN** o Visitante tenta cadastrar outro usuário com login "isac"
- **THEN** o sistema recusa o cadastro informando "Login ja esta em uso."
