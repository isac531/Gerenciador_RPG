# Proposta: ID automático do usuário

## Por quê

Na correção da Entrega 1, o professor pediu que o ID do usuário seja gerado automaticamente. Pedir o ID ao Visitante obriga a pessoa a escolher um número que ela não conhece, gera recusas desnecessárias ("Ja existe um usuario com esse ID.") e não faz sentido para quem só quer criar uma conta.

## O que muda

- O Visitante informa só login, senha e papel. O sistema atribui o ID e o exibe ao final do cadastro.
- Os IDs são sequenciais a partir de 1 e nunca são reaproveitados, nem depois de uma exclusão (o próximo ID é gravado no arquivo binário).
- RF01 deixa de ter os cenários "ID já existente" e "ID não positivo" e ganha os cenários de geração do ID.

## Fora do escopo

- IDs de outras entidades (o personagem já nasce com ID automático na change `crud-personagem`).
- Migração de arquivos `usuarios.dat` da versão anterior: o formato ganhou o campo "próximo ID" e arquivos antigos são recusados como corrompidos (NF-CON-02).

## Impacto

- Código: `Usuario`, `IUsuarioRepositorio::adicionar` (passa a retornar o ID), repositórios, `ControleCriarConta`, `MenuPrincipal`.
- Documentos: UC01, TL02, `08-projeto.md` e [ADR-0003](../../../../docs/07-arquitetura/adr/0003-id-automatico.md).
