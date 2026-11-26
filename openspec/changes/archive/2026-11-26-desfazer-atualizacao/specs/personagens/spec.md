## ADDED Requirements

### Requirement: RF15 — Desfazer a última edição da ficha
O sistema DEVE permitir desfazer a última edição de ficha (RF10) feita na execução atual, voltando o personagem editado à ficha que ele tinha antes, com o mesmo ID e o mesmo dono, e gravando o resultado no armazenamento escolhido (RF05). Só a última edição DEVE poder ser desfeita: depois de desfeita, ou se não houve edição, o sistema informa que não há edição para desfazer.

**Prioridade:** Essencial
**Casos de uso:** UC17, UC12

#### Scenario: Desfazer a edição
- **GIVEN** o personagem de ID 1 tinha nível 3 e 27 PV e a ficha foi editada para nível 4 e 35 PV
- **WHEN** o usuário escolhe "Desfazer ultima edicao de ficha"
- **THEN** o sistema exibe "Edicao desfeita. Ficha restaurada:" com o personagem 1, do mesmo dono, com nível 3 e 27 PV

#### Scenario: Só a última edição
- **GIVEN** a ficha do personagem 1 foi editada do nível 3 para o 4 e depois do 4 para o 5
- **WHEN** o usuário desfaz duas vezes seguidas
- **THEN** a primeira vez volta o personagem ao nível 4 e a segunda exibe "Nenhuma edicao para desfazer." e o personagem continua no nível 4

#### Scenario: Nenhuma edição
- **GIVEN** nenhuma ficha foi editada desde que o sistema foi iniciado
- **WHEN** o usuário escolhe "Desfazer ultima edicao de ficha"
- **THEN** o sistema exibe "Nenhuma edicao para desfazer." e nada é alterado

#### Scenario: Edição recusada não conta
- **GIVEN** a ficha do personagem 1 foi editada do nível 3 para o 4 e, em seguida, uma edição com nome vazio foi recusada
- **WHEN** o usuário desfaz
- **THEN** o personagem 1 volta ao nível 3

#### Scenario: Personagem excluído depois da edição
- **GIVEN** a ficha do personagem 1 foi editada e depois o personagem 1 foi excluído
- **WHEN** o usuário desfaz
- **THEN** o sistema exibe "Nenhuma edicao para desfazer." e nenhum personagem é criado

#### Scenario: Desfazer persistido
- **GIVEN** o sistema foi iniciado com a opção "Arquivo binario" e a ficha do personagem 1 foi editada do nível 3 para o 4 e a edição foi desfeita
- **WHEN** o sistema é fechado e iniciado de novo com a mesma opção
- **THEN** a listagem mostra o personagem 1 com nível 3

#### Scenario: Falha ao gravar ao desfazer
- **GIVEN** a ficha do personagem 1 foi editada e a gravação de `personagens.dat` falha
- **WHEN** o usuário desfaz
- **THEN** o sistema exibe uma mensagem iniciada por "Falha no armazenamento:", o personagem continua com a ficha editada e uma nova tentativa de desfazer ainda é possível
