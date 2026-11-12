# log.c (biblioteca externa)

Cópia sem alterações da biblioteca de log em C [rxi/log.c](https://github.com/rxi/log.c) (versão 0.1.0, licença MIT em [`LICENSE`](LICENSE)).

É uma biblioteca de terceiros: **não edite estes arquivos**. O sistema não a usa diretamente; quem conversa com ela é o adaptador `infra/LogCAdapter`, que a adapta à interface `ILogger` da camada business (padrão Adapter, [ADR-0006](../../../../docs/07-arquitetura/adr/0006-adapter-log.md)).

Ela é compilada como C (`gcc -std=c99`) pelo `src/Makefile`.
