## ADDED Requirements

### Requirement: RF08 — Criar personagem
O sistema DEVE permitir criar um personagem para um usuário existente, informando o ID do usuário dono e a ficha: nome (1 a 30 caracteres), nível (1 a 20), pontos de vida (maior que zero) e os seis atributos (cada um de 1 a 30). O sistema DEVE gerar o ID do personagem automaticamente, sem reaproveitar IDs, e informar o motivo de qualquer recusa.

**Prioridade:** Essencial
**Casos de uso:** UC07

#### Scenario: Criação válida
- **GIVEN** existe o usuário de ID 1 e nenhum personagem foi criado ainda
- **WHEN** é criado o personagem "Thorin" para o usuário 1, nível 3, 27 PV, Força 16, Destreza 12, Constituição 14, Inteligência 8, Sabedoria 10 e Carisma 9
- **THEN** o sistema exibe "Personagem criado com sucesso. ID: 1" e o personagem aparece na listagem

#### Scenario: Dono inexistente
- **GIVEN** não existe usuário de ID 9
- **WHEN** é criado um personagem para o usuário 9
- **THEN** o sistema recusa informando "Nao existe usuario com esse ID para ser o dono." e nada é guardado

#### Scenario: Nível fora do limite
- **WHEN** é criado um personagem de nível 21
- **THEN** o sistema recusa informando "O nivel deve estar entre 1 e 20."

#### Scenario: Atributo fora do limite
- **WHEN** é criado um personagem com Carisma 31
- **THEN** o sistema recusa informando "O atributo Carisma deve estar entre 1 e 30."

#### Scenario: Nome vazio
- **WHEN** é criado um personagem com nome vazio ou só com espaços
- **THEN** o sistema recusa informando "O nome do personagem deve ter de 1 a 30 caracteres."

#### Scenario: Pontos de vida zerados
- **WHEN** é criado um personagem com 0 pontos de vida
- **THEN** o sistema recusa informando "Os pontos de vida devem ser maiores que zero."

### Requirement: RF09 — Listar personagens
O sistema DEVE listar todos os personagens em ordem de ID, exibindo ID, login do dono, nome, nível, pontos de vida e cada atributo com o seu modificador.

**Prioridade:** Essencial
**Casos de uso:** UC13

#### Scenario: Listagem com personagens
- **GIVEN** existe o personagem "Thorin" do usuário "isac" com Força 16 e Destreza 9
- **WHEN** o usuário pede a listagem de personagens
- **THEN** o sistema exibe "Dono: isac", "Nome: Thorin", "FOR 16 (+3)" e "DES 9 (-1)"

#### Scenario: Nenhum personagem
- **GIVEN** não há personagens cadastrados
- **WHEN** o usuário pede a listagem de personagens
- **THEN** o sistema exibe "Nenhum personagem cadastrado."

### Requirement: RF10 — Editar ficha do personagem
O sistema DEVE permitir substituir a ficha de um personagem existente, aplicando as mesmas regras da criação, sem mudar o ID nem o dono.

**Prioridade:** Essencial
**Casos de uso:** UC12

#### Scenario: Edição válida
- **GIVEN** existe o personagem de ID 1, nível 3 e 27 PV, do usuário 1
- **WHEN** a ficha é editada para nível 4 e 35 PV
- **THEN** o sistema exibe "Ficha atualizada com sucesso." e a listagem mostra o personagem 1, ainda do usuário 1, com nível 4 e 35 PV

#### Scenario: Ficha inválida na edição
- **GIVEN** existe o personagem de ID 1 com o nome "Thorin"
- **WHEN** a ficha é editada com nome vazio
- **THEN** o sistema recusa informando "O nome do personagem deve ter de 1 a 30 caracteres." e o personagem continua com o nome "Thorin"

#### Scenario: Personagem inexistente
- **GIVEN** não existe personagem de ID 9
- **WHEN** o usuário pede para editar o personagem 9
- **THEN** o sistema exibe "Personagem nao encontrado." e não pede a ficha

### Requirement: RF11 — Excluir personagem
O sistema DEVE permitir excluir um personagem pelo ID, informando quando o ID não existe.

**Prioridade:** Importante
**Casos de uso:** UC14

#### Scenario: Exclusão de personagem existente
- **GIVEN** existe o personagem de ID 2
- **WHEN** o usuário pede a exclusão do personagem 2
- **THEN** o sistema exibe "Personagem excluido com sucesso." e o personagem some da listagem

#### Scenario: ID inexistente
- **GIVEN** não existe personagem de ID 9
- **WHEN** o usuário pede a exclusão do personagem 9
- **THEN** o sistema exibe "Personagem nao encontrado." e nada é alterado

### Requirement: RF12 — Consultar quantidade de entidades cadastradas
O sistema DEVE informar a quantidade total de entidades cadastradas (usuários mais personagens) e a quantidade de cada tipo.

**Prioridade:** Importante
**Casos de uso:** UC15

#### Scenario: Sistema com usuários e personagens
- **GIVEN** existem 2 usuários e 1 personagem
- **WHEN** o usuário escolhe "Quantidade de entidades cadastradas"
- **THEN** o sistema exibe "Entidades cadastradas: 3 (usuarios: 2, personagens: 1)"

#### Scenario: Sistema vazio
- **GIVEN** não há usuários nem personagens
- **WHEN** o usuário escolhe "Quantidade de entidades cadastradas"
- **THEN** o sistema exibe "Entidades cadastradas: 0 (usuarios: 0, personagens: 0)"
