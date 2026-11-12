## ADDED Requirements

### Requirement: RF13 — Registrar acessos
O sistema DEVE registrar cada tentativa de login (RF07), aceita ou recusada, com o login informado, o usuário correspondente (quando o login existe), a data e hora e o resultado, sem guardar a senha digitada. Os registros DEVEM usar o mesmo armazenamento escolhido no início (RF05) e nunca podem ser alterados ou excluídos pelo sistema.

**Prioridade:** Essencial
**Casos de uso:** UC02, UC16

#### Scenario: Login aceito
- **GIVEN** existe o usuário "isac" (ID 1) com senha "Dado#2026"
- **WHEN** ele entra com login "isac" e senha "Dado#2026"
- **THEN** o sistema exibe "Bem-vindo, isac (Mestre)." e guarda um registro de acesso aceito do usuário 1 com a data e hora atuais

#### Scenario: Senha incorreta
- **GIVEN** existe o usuário "isac" (ID 1)
- **WHEN** alguém entra com login "isac" e senha "Errada#99"
- **THEN** o sistema exibe "Login ou senha invalidos." e guarda um registro de acesso recusado do usuário 1

#### Scenario: Login inexistente
- **GIVEN** não existe o usuário "fantasma"
- **WHEN** alguém entra com login "fantasma"
- **THEN** o sistema exibe "Login ou senha invalidos." e guarda um registro de acesso recusado sem usuário

#### Scenario: Registros persistidos
- **GIVEN** o sistema foi iniciado com a opção "Arquivo binario" e houve 3 tentativas de login
- **WHEN** o sistema é fechado e iniciado de novo com a mesma opção
- **THEN** o relatório de acessos mostra 3 tentativas de login

### Requirement: RF14 — Gerar relatório de acessos
O sistema DEVE gerar um relatório de estatísticas de acesso no formato escolhido pelo usuário, HTML ou PDF, gravado na pasta de execução como `relatorio-acessos.html` ou `relatorio-acessos.pdf`. Os dois formatos DEVEM ter as mesmas seções, na mesma ordem, e os mesmos números.

**Prioridade:** Essencial
**Casos de uso:** UC16

#### Scenario: Seções do relatório
- **WHEN** o usuário gera o relatório em qualquer formato
- **THEN** o arquivo tem, nesta ordem: título, data e hora de geração, resumo (tentativas de login, acessos aceitos, tentativas recusadas, recusas com login inexistente, taxa de sucesso, usuários cadastrados, usuários que já acessaram, usuário mais ativo, primeira e última tentativa) e a tabela com ID, login, papel, acessos, recusas e último acesso de cada usuário cadastrado

#### Scenario: Relatório em HTML
- **GIVEN** o usuário "isac" teve 2 acessos aceitos e 1 recusado, o usuário "bia" nenhum, e houve 1 tentativa com login inexistente
- **WHEN** o usuário escolhe "Relatorio de acessos" e o formato HTML
- **THEN** o sistema exibe "Relatorio gravado em relatorio-acessos.html." e o arquivo mostra 4 tentativas, 2 aceitas, 2 recusadas, taxa de sucesso de 50.0%, "isac (2)" como usuário mais ativo e a linha de "bia" com 0 acessos e último acesso "-"

#### Scenario: Relatório em PDF
- **GIVEN** os mesmos acessos do cenário anterior
- **WHEN** o usuário escolhe o formato PDF
- **THEN** o sistema exibe "Relatorio gravado em relatorio-acessos.pdf." e o arquivo abre em um leitor de PDF com as mesmas seções e os mesmos números do HTML

#### Scenario: Sem acessos
- **GIVEN** nenhuma tentativa de login foi feita
- **WHEN** o usuário gera o relatório
- **THEN** o resumo mostra 0 tentativas, taxa de sucesso de 0.0% e "-" como usuário mais ativo

#### Scenario: Formato inválido
- **WHEN** o usuário escolhe o formato 3
- **THEN** o sistema exibe "Formato invalido." e nenhum arquivo é gravado
