# Proposta: CRUD de personagens (Sprint 3)

## Por quê

A Sprint 3 pede o cadastro completo (CRUD) de uma entidade do domínio relacionada a Usuário, e uma fachada única (Facade + Singleton) para os controles, com um método que retorna a quantidade de entidades cadastradas. O **Personagem** é a entidade central do jogo depois do usuário: a visão (§2.3) prevê que Jogadores criem e editem fichas, e o diagrama de classes de análise já liga `Usuario 1 → * Personagem`.

## O que muda

- **ADDED** RF08 Criar personagem, RF09 Listar personagens, RF10 Editar ficha do personagem, RF11 Excluir personagem e RF12 Consultar a quantidade de entidades cadastradas.
- **MODIFIED** RF05: o armazenamento escolhido vale também para os personagens (`personagens.dat`).
- **MODIFIED** RF06: excluir um usuário exclui também os personagens dele.
- Casos de uso: UC07 passa a ser **Criar personagem**; entram UC12 Editar ficha, UC13 Listar personagens, UC14 Excluir personagem e UC15 Consultar quantidade de entidades.
- Código: entidade `Personagem` com `Ficha`, regras de validação da ficha, repositórios em memória e em arquivo binário, `ControlePersonagem`, `FacadeSingletonController` e o menu de personagens.

## Fora do escopo

- Associar personagem a campanha (UC10), rolagens (UC08) e NPCs (UC11).
- Restringir o CRUD ao dono do personagem: depende da autenticação (RF07, UC02), que ainda não foi implementada. Nesta sprint quem usa o menu informa o ID do usuário dono.
- Bônus de proficiência, classe de armadura e demais valores da ficha que não são nome, nível, pontos de vida e atributos.

## Impacto

- Specs: nova capacidade `personagens`; `usuarios` modificada (RF05, RF06).
- Documentos: casos de uso, telas TL03–TL05, `05-analise.md`, `07-arquitetura/`, `08-projeto.md`, [ADR-0004](../../../../docs/07-arquitetura/adr/0004-fachada-singleton.md).
