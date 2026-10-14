# rnf/seguranca Specification

## Purpose
Requisitos não funcionais de proteção das credenciais dos usuários e de controle de acesso.

## Requirements

### Requirement: NF-SEG-01 — Senha nunca exibida
O sistema NÃO DEVE exibir a senha em listagens, mensagens de erro ou mensagens de confirmação.

**Prioridade:** Essencial
**Casos de uso:** UC01, UC03

#### Scenario: Erro de validação de senha
- **GIVEN** o Visitante digitou a senha "dragaovermelho"
- **WHEN** o cadastro é recusado por senha inválida
- **THEN** a mensagem descreve a regra violada e contém 0 ocorrências do texto "dragaovermelho"

#### Scenario: Listagem
- **GIVEN** existem 10 usuários cadastrados
- **WHEN** a listagem é exibida
- **THEN** a saída contém 0 ocorrências de qualquer uma das 10 senhas

### Requirement: NF-SEG-02 — Falha de login sem pista
O sistema DEVE responder a uma tentativa de login incorreta com a mesma mensagem, quer o erro esteja no login, quer na senha.

**Prioridade:** Importante
**Casos de uso:** UC02

#### Scenario: Login inexistente e senha errada
- **GIVEN** existe o usuário "isac"
- **WHEN** alguém tenta entrar com o login "kaio" (inexistente) e depois com "isac" e uma senha errada
- **THEN** as 2 tentativas recebem exatamente a mesma mensagem "Login ou senha invalidos."
