# Gerenciador de RPG

[![Validar especificação](https://github.com/isac531/Gerenciador_RPG/actions/workflows/openspec.yml/badge.svg)](https://github.com/isac531/Gerenciador_RPG/actions/workflows/openspec.yml)

Sistema para gerenciar campanhas de RPG de mesa (D&D 5e): Mestres criam campanhas, Jogadores entram com seus personagens, e o sistema automatiza a parte matemática do jogo (rolagem de dados, pontos de vida, modificadores e bônus), **sem loja e sem anúncios**.

Projeto da disciplina **Métodos e Projeto de Software** (UFPB · Centro de Informática), documentado com o modelo [`rkulesza/mps-modelo`](https://github.com/rkulesza/mps-modelo) e o [OpenSpec](https://github.com/Fission-AI/OpenSpec).

**Prazos e o que entregar em cada sprint: [`docs/00-entregas.md`](docs/00-entregas.md).** Regras de branches, commits, PRs e código: [`CONTRIBUTING.md`](CONTRIBUTING.md).

---

## 1. Como compilar e executar

Requisitos: g++ com suporte a C++17 e `make` (no Windows, MinGW).

```bash
cd src
make          # compila o executável rpg
./rpg         # executa (no Windows: rpg.exe)
make test     # compila e executa os testes automatizados
```

Ao iniciar, o sistema pergunta onde guardar os usuários (RF05):

```text
Escolha o armazenamento:
1. Memoria (RAM)
2. Arquivo binario (usuarios.dat)
```

- **Memória:** os usuários ficam numa coleção em RAM e se perdem ao fechar o programa.
- **Arquivo binário:** os usuários são carregados de `usuarios.dat` (na pasta de onde o programa é executado) e cada cadastro ou exclusão é gravado na hora. Falhas de leitura ou gravação são tratadas e informadas sem encerrar o programa (NF-CON-01).

As senhas são gravadas sem criptografia no arquivo; não use senhas reais.

---

## 2. Onde está cada seção do documento de requisitos

| Seção do modelo | Neste repositório |
|---|---|
| Capa (nome do projeto e integrantes) | Este `README.md` (seção 5) |
| Histórico de revisões | Histórico do Git: commits, pull requests e `openspec/changes/archive/` |
| 1. Introdução | [`docs/01-visao.md`](docs/01-visao.md) §1 |
| 2. Descrição geral | [`docs/01-visao.md`](docs/01-visao.md) §2 |
| 3. Glossário | [`docs/02-glossario.md`](docs/02-glossario.md) |
| 4. Elicitação de requisitos | [`docs/03-elicitacao.md`](docs/03-elicitacao.md) + [`docs/03-elicitacao/anexos/`](docs/03-elicitacao/anexos/) |
| 5.1 Requisitos funcionais | [`openspec/specs/usuarios/spec.md`](openspec/specs/usuarios/spec.md) |
| 5.2 Requisitos não funcionais | [`openspec/specs/rnf/`](openspec/specs/rnf/) (segurança, confiabilidade, manutenibilidade, usabilidade, desempenho, portabilidade) |
| 6. Casos de uso | [`docs/04-casos-de-uso/`](docs/04-casos-de-uso/) |
| 7. Diagrama de classes de análise | [`docs/05-analise.md`](docs/05-analise.md) |
| 8. Descrição da interface | [`docs/06-interface/`](docs/06-interface/) |
| 9. Arquitetura | [`docs/07-arquitetura/`](docs/07-arquitetura/) + ADRs em `adr/` |
| Pacotes, classes de projeto e padrões | [`docs/08-projeto.md`](docs/08-projeto.md) |

### Classificação dos requisitos não funcionais

| Tipo (Sommerville) | Requisitos |
|---|---|
| **Produto**: como o sistema deve se comportar | NF-SEG-01, NF-SEG-02 (segurança); NF-CON-01 a NF-CON-03 (confiabilidade); NF-DES-01 (desempenho); NF-USA-01 a NF-USA-03 (usabilidade); NF-POR-02 (portabilidade dos dados) |
| **Organizacional**: decorrem de políticas e do processo da equipe | NF-MAN-01 a NF-MAN-03 (manutenibilidade e testes); NF-POR-01 (C++17 com g++, sem bibliotecas externas) |
| **Externo**: vêm de fora do sistema | Regras de senha da política padrão do AWS IAM (RF03, restrição R2); regras de jogo do SRD 5.1 (restrição R4) |

---

## 3. Estrutura

```text
.
├── README.md                     ← capa, como executar, mapa da documentação
├── CONTRIBUTING.md               ← branches, commits, PRs, clean code
├── docs/                         ← visão, glossário, elicitação, casos de uso, diagramas
│   └── diagramas/                ← diagramas originais (PDF/PNG)
├── openspec/
│   ├── config.yaml
│   ├── specs/usuarios/           ← requisitos funcionais (RF01–RF07)
│   ├── specs/rnf/<categoria>/    ← requisitos não funcionais
│   └── changes/                  ← mudanças de requisito a partir da Entrega 2
├── src/                          ← código C++17 (ui, business, infra) e Makefile
└── .github/                      ← validação das specs no GitHub Actions, modelo de PR
```

---

## 4. Convenções

| Item | Formato | Exemplo |
|---|---|---|
| Requisito funcional | `RFnn` | RF03 |
| Requisito não funcional | `NF-CAT-nn` | NF-SEG-01 |
| Caso de uso | `UCnn` | UC01 |
| Tela | `TLnn` | TL01 |
| Decisão de arquitetura | `ADR-nnnn` | ADR-0001 |

IDs são estáveis: nunca são renumerados nem reaproveitados. Valide as specs antes de cada commit:

```bash
npm install -g @fission-ai/openspec@1   # uma vez (Node.js 20.19+)
openspec validate --all
```

O aviso *"should contain SHALL or MUST"* é esperado: as specs são escritas em português, com **DEVE**.

---

## 5. Equipe

**Sistema:** Gerenciador de RPG

| Integrante | Matrícula | GitHub |
|---|---|---|
| Allejandro Sousa dos Santos | 20220006054 | [@AllejandroSousa](https://github.com/AllejandroSousa) |
| Gustavo Gomes Souta da Silva | 20220023959 | [@IGustaMe](https://github.com/IGustaMe) |
| Isac Santos Lira | 20230047257 | [@isac531](https://github.com/isac531) |
| Kaio César de Oliveira Barreto | 20220060540 | [@kaiocesarb15](https://github.com/kaiocesarb15) |
