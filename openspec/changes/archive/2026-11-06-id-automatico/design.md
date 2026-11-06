# Design: ID automático do usuário

Decisão registrada no [ADR-0003](../../../../docs/07-arquitetura/adr/0003-id-automatico.md).

- Quem gera o ID é o **repositório** (`IUsuarioRepositorio::adicionar` retorna o ID gerado), porque é ele que sabe quais IDs já existem em cada mecanismo de armazenamento.
- Cada repositório guarda um contador `proximoId_`. O arquivo binário passa a começar com esse contador, para que um ID excluído não volte a ser usado depois de reiniciar o sistema.
- `Usuario` ganha um construtor sem ID (o objeto nasce com `SEM_ID = 0`) e o método `comId(id)`, que devolve uma cópia com o ID atribuído. A entidade continua imutável.
- `ControleCriarConta::criarConta` retorna o ID gerado, que a interface exibe.

Novo formato de `usuarios.dat` (little-endian): `int32` próximo ID, `uint32` quantidade e, para cada usuário, `int32` ID, login, senha e papel como antes.
