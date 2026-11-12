# [UC16] Gerar relatório de acessos

Este caso de uso é responsável por gerar, em HTML ou em PDF, um relatório com as estatísticas de acesso dos usuários ao sistema, calculadas a partir das tentativas de login registradas (RF13).

| Campo | Valor |
|---|---|
| **Ator** | Usuário (Mestre ou Jogador) |
| **Prioridade** | Essencial |
| **Requisitos** | RF14, RF13, RF05, NF-USA-03 |
| **Interfaces** | menu principal (opção 7) |

**Entradas e pré-condições**
- O armazenamento já foi escolhido ao iniciar o sistema (TL01).
- Dado de entrada: o formato do relatório (1 = HTML, 2 = PDF).

**Saídas e pós-condições**
- Sucesso: o arquivo `relatorio-acessos.html` ou `relatorio-acessos.pdf` é gravado (ou substituído) na pasta de execução, com as seções do RF14.
- Falha: nenhum arquivo novo é gravado e o Usuário vê o motivo.

## Fluxo principal

1. Este caso de uso se inicia quando o Usuário escolhe "Relatorio de acessos" no menu principal.
2. O sistema pede o formato: "1. HTML  2. PDF".
3. O Usuário informa o formato [FE1].
4. O sistema calcula as estatísticas a partir dos registros de acesso e dos usuários cadastrados: tentativas, acessos aceitos e recusados, taxa de sucesso, usuário mais ativo, primeira e última tentativa e, para cada usuário, acessos, recusas e último acesso.
5. O sistema monta o relatório no formato escolhido, com as mesmas seções e na mesma ordem para qualquer formato.
6. O sistema grava o arquivo [FE2] e exibe "Relatorio gravado em relatorio-acessos.<extensão>.".
7. O sistema volta ao menu principal.

## Fluxos secundários

**FE1 — Formato inválido (passo 3)**
1. Caso o Usuário informe algo diferente de 1 ou 2, o sistema exibe "Formato invalido." e não gera relatório.
2. O caso de uso volta ao menu principal.

**FE2 — Falha ao gravar o arquivo (passo 6)**
1. Caso o arquivo não possa ser gravado (pasta sem permissão, disco cheio), o sistema exibe "Nao foi possivel gravar o arquivo relatorio-acessos.<extensão>.".
2. O caso de uso volta ao menu principal, sem encerrar o programa.
