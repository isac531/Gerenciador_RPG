# rnf/desempenho Specification

## Purpose
Requisitos não funcionais de tempo de resposta das operações de usuários.

## Requirements

### Requirement: NF-DES-01 — Tempo de listagem e carga
O sistema DEVE listar todos os usuários e carregar o arquivo binário na inicialização em até 1 segundo cada, com até 10.000 usuários cadastrados, num computador de laboratório comum.

**Prioridade:** Desejável
**Casos de uso:** UC03

#### Scenario: Base com 10.000 usuários
- **GIVEN** um arquivo binário com 10.000 usuários
- **WHEN** o sistema é iniciado e a listagem é solicitada
- **THEN** a carga e a listagem terminam, cada uma, em até 1 segundo
