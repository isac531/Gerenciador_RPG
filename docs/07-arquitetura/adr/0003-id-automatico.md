# ADR-0003: Gerar o ID do usuário no repositório

- **Status:** aceito
- **Data:** 2026-11-06
- **Requisitos relacionados:** RF01, NF-CON-02

## Contexto

Até a Entrega 1 o Visitante digitava o ID do usuário, e o sistema recusava IDs não positivos ou repetidos. Na correção, o professor pediu ID automático. A mudança altera o contrato público de `IUsuarioRepositorio` e de `ControleCriarConta`, por isso é registrada aqui (CONTRIBUTING §6.3).

## Decisão

O **repositório** gera o ID ao guardar: `int adicionar(const Usuario&)` atribui o próximo ID livre, guarda uma cópia do usuário com esse ID (`Usuario::comId`) e o retorna. Os IDs começam em 1, são sequenciais e nunca são reaproveitados: cada repositório mantém um contador `proximoId_`, que o arquivo binário grava no início do arquivo. O controle repassa o ID gerado para a interface, que o exibe.

## Alternativas consideradas

| Alternativa | Por que não |
|---|---|
| Gerar o ID no controle (maior ID + 1) | O controle teria de conhecer todos os IDs; e, ao excluir o último usuário, o ID dele voltaria a ser usado, o que pode ligar personagens antigos a um usuário novo |
| Contador estático na classe `Usuario` | Não sobrevive a reinícios com o arquivo binário e mistura regra de armazenamento com a entidade |
| UUID | Desnecessário para um sistema local de console e difícil de digitar no menu |

## Consequências

- Positivas: o Visitante não precisa escolher um número; somem as recusas por ID; o mesmo padrão serve para o personagem.
- Negativas: arquivos `usuarios.dat` gravados pela versão anterior são recusados como corrompidos (NF-CON-02); basta apagá-los, pois não há dados reais em produção.
