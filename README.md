# Gerenciador-RPG

## Armazenamento de usuarios

Os usuarios sao carregados de `usuarios.txt` quando o programa inicia. O arquivo fica no diretorio de trabalho atual (a pasta de onde o programa e executado). Se ele ainda nao existir, o sistema comeca sem usuarios e o cria no primeiro cadastro.

Cada linha contem o ID, o nome e a senha entre aspas, por exemplo:

```text
1 "Maria Silva" "Senha@123"
```

O formato entre aspas permite nomes e senhas com espacos. Cada cadastro ou exclusao atualiza o arquivo imediatamente; assim, os dados continuam disponiveis na proxima execucao. Registros malformados ou IDs duplicados impedem a carga para evitar ignorar dados silenciosamente.

**Seguranca:** as senhas sao gravadas em texto simples. Restrinja o acesso ao arquivo e nao use este armazenamento em um ambiente de producao sem adotar uma solucao apropriada para proteger credenciais.