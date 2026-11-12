# Design: relatório de acessos

## Registro de acesso

`RegistroAcesso` guarda o login digitado, o ID do usuário (ou `SEM_ID` se o login não existe), a data e hora em segundos desde 1970 (UTC) e se a tentativa foi aceita. O login é guardado junto com o ID para que o histórico continue legível mesmo depois de o usuário ser excluído. O histórico só cresce: `IAcessoRepositorio` tem `adicionar`, `listarTodos`, `listarPorLogin` e `quantidade`, sem alterar nem remover.

`ControleLogin` busca o usuário por login (`IUsuarioRepositorio::buscarPorLogin`, novo), confere a senha, grava o registro e só então lança `CredenciaisInvalidasException` se a tentativa foi recusada. O relógio é injetado para os testes.

## Persistência (Repository + Factory Method)

Ver [ADR-0005](../../../docs/07-arquitetura/adr/0005-repository-e-factory-method.md). `FabricaRepositorio` ganha `criarRepositorioAcessos()`; `FabricaMemoria` cria `RepositorioAcessoMemoria` e `FabricaArquivoBinario` cria `RepositorioAcessoArquivoBinario` (`acessos.dat`).

Formato de `acessos.dat` (little-endian): `int32` próximo ID, `uint32` quantidade; para cada registro, `int32` ID, `uint32` tamanho + bytes do login, `int32` ID do usuário, `int64` data e hora, `uint8` resultado (0 ou 1).

## Estatísticas

`ControleRelatorioAcessos::calcularEstatisticas()` produz `EstatisticasAcesso`:

| Indicador | Cálculo |
|---|---|
| Tentativas de login | todos os registros |
| Acessos aceitos / tentativas recusadas | registros com e sem sucesso |
| Recusas com login inexistente | recusas com `SEM_ID` |
| Taxa de sucesso | aceitos ÷ tentativas × 100, com uma casa decimal |
| Usuários cadastrados / que já acessaram | usuários atuais / com pelo menos um acesso aceito |
| Usuário mais ativo | mais acessos aceitos; empate fica com o menor ID |
| Primeira e última tentativa | menor e maior data e hora |
| Por usuário | ID, login, papel, acessos, recusas e último acesso aceito de cada usuário cadastrado |

Registros de usuários já excluídos entram só nos totais.

## Formatos (Template Method)

Ver [ADR-0007](../../../docs/07-arquitetura/adr/0007-relatorios-template-method.md). `GeradorRelatorioAcessos::gerar()` fixa as seções; `RelatorioHtml` e `RelatorioPdf` escrevem cada uma. O menu grava `relatorio-acessos.html` ou `relatorio-acessos.pdf` na pasta de execução.

## Log (Adapter)

Ver [ADR-0006](../../../docs/07-arquitetura/adr/0006-adapter-log.md). A fachada registra em `rpg.log` os logins aceitos e recusados, cadastros, exclusões, relatórios gerados e falhas de armazenamento, nunca a senha.
