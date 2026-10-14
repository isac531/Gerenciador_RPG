# usuarios Specification

## Purpose
Gerenciamento dos usuários do sistema (Mestre e Jogador): cadastro com validação de login e senha, listagem, exclusão, autenticação e escolha do armazenamento (memória ou arquivo binário). É o núcleo essencial das Sprints 1 e 2.

## Requirements

### Requirement: RF01 — Cadastrar usuário
O sistema DEVE permitir que um Visitante crie uma conta informando ID (número inteiro positivo), login, senha e papel (Mestre ou Jogador), recusando IDs e logins já existentes e informando o motivo de qualquer recusa.

**Prioridade:** Essencial
**Casos de uso:** UC01

#### Scenario: Cadastro válido
- **GIVEN** não existe usuário com ID 1 nem com login "isac"
- **WHEN** o Visitante cadastra ID 1, login "isac", senha "Dado#2026" e papel Mestre
- **THEN** o usuário é armazenado, o sistema exibe "Usuario cadastrado com sucesso." e ele passa a aparecer na listagem

#### Scenario: Login já existente
- **GIVEN** já existe um usuário com login "isac"
- **WHEN** o Visitante tenta cadastrar outro usuário com login "isac"
- **THEN** o sistema recusa o cadastro informando "Login ja esta em uso."

#### Scenario: ID já existente
- **GIVEN** já existe um usuário com ID 1
- **WHEN** o Visitante tenta cadastrar outro usuário com ID 1
- **THEN** o sistema recusa o cadastro informando "Ja existe um usuario com esse ID."

#### Scenario: ID não positivo
- **WHEN** o Visitante tenta cadastrar um usuário com ID 0
- **THEN** o sistema recusa o cadastro informando "O ID deve ser um numero positivo."

### Requirement: RF02 — Regras de login
O sistema DEVE aceitar apenas logins não vazios, com no máximo 12 caracteres e sem números, recusando o cadastro com uma exceção de login inválido que informa a regra violada.

**Prioridade:** Essencial
**Casos de uso:** UC01

#### Scenario: Login vazio
- **WHEN** o Visitante tenta cadastrar um usuário com login vazio ou só com espaços
- **THEN** o sistema recusa o cadastro informando "O login nao pode ser vazio."

#### Scenario: Login com mais de 12 caracteres
- **WHEN** o Visitante informa o login "aventureirosx" (13 caracteres)
- **THEN** o sistema recusa o cadastro informando "O login deve ter no maximo 12 caracteres."

#### Scenario: Login com exatamente 12 caracteres
- **WHEN** o Visitante informa o login "aventureiros" (12 caracteres) e os demais dados válidos
- **THEN** o cadastro é aceito

#### Scenario: Login com números
- **WHEN** o Visitante informa o login "mestre2"
- **THEN** o sistema recusa o cadastro informando "O login nao pode conter numeros."

### Requirement: RF03 — Regras de senha
O sistema DEVE aceitar apenas senhas que sigam a política de senha padrão do AWS IAM: de 8 a 128 caracteres, com pelo menos 3 dos 4 tipos de caractere (maiúscula, minúscula, número, caractere não alfanumérico) e diferente do login, recusando o cadastro com uma exceção de senha inválida que informa a regra violada.

Caracteres não alfanuméricos aceitos: `! @ # $ % ^ & * ( ) _ + - = [ ] { } | '`

**Prioridade:** Essencial
**Casos de uso:** UC01

#### Scenario: Senha curta
- **WHEN** o Visitante informa a senha "Ab#1" (4 caracteres)
- **THEN** o sistema recusa o cadastro informando "A senha deve ter entre 8 e 128 caracteres."

#### Scenario: Senha longa demais
- **WHEN** o Visitante informa uma senha válida nas demais regras com 129 caracteres
- **THEN** o sistema recusa o cadastro informando "A senha deve ter entre 8 e 128 caracteres."

#### Scenario: Senha com poucos tipos de caractere
- **WHEN** o Visitante informa a senha "dragaovermelho" (só minúsculas)
- **THEN** o sistema recusa o cadastro informando "A senha deve conter ao menos 3 dos 4 tipos: maiusculas, minusculas, numeros e especiais."

#### Scenario: Senha com três tipos
- **WHEN** o Visitante informa a senha "dragao#2026" (minúsculas, símbolo e números)
- **THEN** a senha é aceita

#### Scenario: Senha igual ao login
- **WHEN** o Visitante informa login "Elf_Ranger#" e senha "Elf_Ranger#" (válida nas demais regras)
- **THEN** o sistema recusa o cadastro informando "A senha nao pode ser igual ao login."

### Requirement: RF04 — Listar usuários
O sistema DEVE listar todos os usuários cadastrados, exibindo ID, login e papel de cada um, sem exibir a senha.

**Prioridade:** Essencial
**Casos de uso:** UC03

#### Scenario: Listagem com usuários
- **GIVEN** existem 3 usuários cadastrados
- **WHEN** o usuário solicita a listagem
- **THEN** o sistema exibe os 3 usuários com ID, login e papel, sem a senha

#### Scenario: Nenhum usuário
- **GIVEN** não há usuários cadastrados
- **WHEN** o usuário solicita a listagem
- **THEN** o sistema exibe "Nenhum usuario cadastrado."

### Requirement: RF05 — Escolher o armazenamento
O sistema DEVE perguntar, no início da execução, onde os usuários serão armazenados: em memória (RAM) ou em arquivo binário; no arquivo binário, os usuários já gravados DEVEM ser carregados ao iniciar.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC03, UC04

#### Scenario: Armazenamento em memória
- **GIVEN** o sistema é iniciado com a opção "Memoria"
- **WHEN** 2 usuários são cadastrados, o sistema é fechado e iniciado de novo com a opção "Memoria"
- **THEN** a listagem exibe "Nenhum usuario cadastrado."

#### Scenario: Armazenamento em arquivo binário
- **GIVEN** o sistema é iniciado com a opção "Arquivo binario"
- **WHEN** 2 usuários são cadastrados, o sistema é fechado e iniciado de novo com a mesma opção
- **THEN** a listagem exibe os mesmos 2 usuários

### Requirement: RF06 — Excluir usuário
O sistema DEVE permitir excluir um usuário pelo ID, informando quando o ID não existe.

**Prioridade:** Importante
**Casos de uso:** UC04

#### Scenario: Exclusão de usuário existente
- **GIVEN** existe o usuário de ID 2
- **WHEN** o usuário pede a exclusão do ID 2
- **THEN** o sistema exibe "Usuario excluido com sucesso." e o ID 2 some da listagem

#### Scenario: ID inexistente
- **GIVEN** não existe usuário de ID 9
- **WHEN** o usuário pede a exclusão do ID 9
- **THEN** o sistema exibe "Usuario nao encontrado." e nada é alterado

### Requirement: RF07 — Autenticar com login e senha
O sistema DEVE autenticar usuários por login e senha e liberar apenas as funcionalidades do seu papel (Mestre ou Jogador).

**Prioridade:** Importante
**Casos de uso:** UC02

#### Scenario: Credenciais corretas
- **GIVEN** existe o usuário "isac" com papel Mestre
- **WHEN** ele informa login e senha corretos
- **THEN** o sistema abre o menu com as ações de Mestre

#### Scenario: Credenciais incorretas
- **WHEN** o usuário informa uma senha incorreta
- **THEN** o sistema nega o acesso com a mensagem "Login ou senha invalidos.", sem indicar qual dos dois está errado
