# Visão do sistema — Gerenciador de RPG

> Equivale às seções 1 (Introdução) e 2 (Descrição geral) do modelo de documento de requisitos.

## 1. Introdução

### 1.1 Propósito deste repositório

Este repositório especifica e implementa o **Gerenciador de RPG**, um sistema para organizar campanhas de RPG de mesa (como *Dungeons & Dragons* 5ª edição) e automatizar a parte matemática do jogo: rolagem de dados, cálculo de pontos de vida, modificadores e bônus. Ele é destinado à equipe de desenvolvimento e ao professor da disciplina Métodos e Projeto de Software.

### 1.2 Organização

| Onde | O quê |
|---|---|
| `docs/01-visao.md` | Este arquivo: motivação, problemas, escopo, usuários, restrições |
| `docs/02-glossario.md` | Termos do domínio |
| `docs/03-elicitacao.md` | Técnicas de elicitação usadas e seus registros |
| `openspec/specs/` | **Requisitos funcionais e não funcionais** (fonte da verdade) |
| `docs/04-casos-de-uso/` | Diagrama de casos de uso e casos de uso detalhados |
| `docs/05-analise.md` | Diagrama de classes de análise |
| `docs/06-interface/` | Rascunhos de tela |
| `docs/07-arquitetura/` | Arquitetura e decisões (ADRs) |
| `docs/08-projeto.md` | Pacotes, diagrama de classes de projeto e padrões |
| `src/` | Código-fonte (C++17) |

### 1.3 Documentos relacionados

1. *Player's Handbook* (D&D 5ª edição), Wizards of the Coast, 2014: regras de atributos, modificadores, testes e pontos de vida usadas como referência de domínio.
2. *System Reference Document 5.1* (SRD 5.1), Wizards of the Coast, licença Creative Commons CC-BY-4.0: versão aberta das regras básicas de D&D 5e.
3. Política de senha padrão do AWS IAM: <https://docs.aws.amazon.com/pt_br/IAM/latest/UserGuide/id_credentials_passwords_account-policy.html> (regras de senha, RF03).
4. [Glossário](02-glossario.md) e [registro da elicitação](03-elicitacao.md).

## 2. Descrição geral

### 2.1 Motivação

Grupos de RPG usam plataformas on-line para guardar fichas de personagem e acompanhar campanhas. A mais conhecida, o **D&D Beyond**, mistura o gerenciamento da campanha com uma loja de livros digitais e anúncios, e sua interface é carregada e difícil de navegar. Alternativas como Roll20 e Foundry VTT são completas, mas focam em mapas e mesa virtual, exigem configuração e têm curva de aprendizado alta.

Durante a sessão, boa parte do tempo vai para contas repetitivas: somar o modificador de atributo à rolagem, aplicar bônus de proficiência, descontar dano dos pontos de vida. Erros nessas contas atrasam o jogo e geram discussões.

O Gerenciador de RPG quer ser uma ferramenta **simples, sem loja e sem anúncios**, em que o Mestre cria a campanha, os Jogadores entram com seus personagens e o sistema faz as contas.

### 2.2 Problemas identificados

| # | Problema | Quem é afetado | Evidência |
|---|---|---|---|
| P1 | Anúncios e ofertas da loja atrapalham o uso da ferramenta de campanha, até para assinantes | Mestres, Jogadores | Fórum oficial do D&D Beyond, 3 tópicos de 2020 a 2026 (elicitação §4.2, D1–D3); relato da equipe |
| P2 | Interfaces carregadas e pouco intuitivas, com curva de aprendizado alta para iniciantes | Mestres, Jogadores novatos | Teste de usabilidade do Roll20 e avaliações de Roll20 e Foundry VTT (§4.2, D5–D7); página inicial do D&D Beyond (D3) |
| P3 | Cálculo manual de modificadores, bônus e pontos de vida toma tempo e gera erros | Jogadores, Mestres | Relato da equipe; as três ferramentas com ficha automatizam esses cálculos (§4.1) |
| P4 | Rolagens feitas fora de uma ferramenta compartilhada não ficam registradas para o grupo | Mestres | Benchmarking: todas as ferramentas analisadas integram a rolagem (§4.1) |
| P5 | As soluções completas cobram assinatura, licença ou exigem servidor próprio | Grupos de estudantes | Benchmarking (§4.1); relatos sobre custo (§4.2, D4, D6, D7) |

### 2.3 Visão geral do sistema

**O sistema fará:**
- Gerenciamento de usuários com dois papéis, **Mestre** e **Jogador**: cadastro com validação de login e senha, listagem, exclusão e autenticação (núcleo das Entregas 1 e 2).
- Criação e gerenciamento de **campanhas** pelo Mestre, com limite de jogadores.
- Criação e edição de **personagens** (fichas) pelos Jogadores, com atributos, nível e pontos de vida.
- Entrada de Jogadores em campanhas, associando um personagem à campanha.
- Cadastro de **NPCs** pelo Mestre.
- **Rolagem de dados** (D4, D6, D8, D10, D12, D20, D100) com modificadores aplicados automaticamente e histórico na campanha.
- Cálculo automático de modificadores de atributo, bônus e pontos de vida.
- **Escudo do Mestre**: visão do Mestre com informações da campanha que os Jogadores não veem.

**O sistema NÃO fará (escopo negativo):**

| Funcionalidade | Motivo |
|---|---|
| Loja, venda de livros ou conteúdo pago | É justamente o que torna o D&D Beyond confuso (P1); o sistema é gratuito |
| Anúncios | Mesmo motivo (P1) |
| Mapas táticos, grid e tokens (mesa virtual) | Já resolvido por Roll20, Foundry VTT e Owlbear Rodeo; aumentaria muito o escopo |
| Chat de voz ou vídeo | Os grupos já usam Discord |
| Conteúdo oficial protegido por direitos autorais (livros, magias e monstros fora do SRD) | Restrição legal; o usuário digita o que precisar |
| Suporte a vários sistemas de regras (Pathfinder, Tormenta etc.) nesta versão | O cálculo automático segue D&D 5e; outros sistemas ficam para versões futuras |

**Integração com outros sistemas:** nesta versão o sistema é **autocontido**: não depende de serviços externos. Os dados ficam em memória ou em arquivo binário local.

### 2.4 Usuários do sistema

| Usuário | Características | Principais dificuldades hoje |
|---|---|---|
| **Mestre** | Conduz a campanha, cria a história, controla NPCs e decide os testes; costuma ter mais experiência com o sistema de regras; usa a ferramenta antes (preparação) e durante a sessão | Acompanhar a ficha de vários jogadores ao mesmo tempo; conferir rolagens; ferramentas cheias de distrações |
| **Jogador** | Controla um personagem por campanha; perfis variados, de novatos a veteranos; usa a ferramenta principalmente durante a sessão, muitas vezes pelo celular ou notebook | Fazer as contas da ficha sem errar; entender interfaces complexas |
| **Visitante** | Pessoa ainda sem conta; só pode criar uma conta | — |

Todo Mestre e todo Jogador é um **Usuário** com login e senha. O papel é escolhido no cadastro.

### 2.5 Suposições e restrições gerais

- **R1** — Linguagem C++17, compilada com g++; nesta fase a interface é de **console** (texto).
- **R2** — Login segue as regras da disciplina (não vazio, até 12 caracteres, sem números) e a senha segue a política padrão do AWS IAM (RF02, RF03). A senha nunca é exibida (NF-SEG-01).
- **R3** — O armazenamento é escolhido no início da execução: memória (RAM) ou arquivo binário (RF05, NF-MAN-01).
- **R4** — Regras de cálculo seguem o SRD 5.1 de D&D 5e.
- **R5** — Projeto acadêmico, sem orçamento: nenhuma dependência paga ou serviço externo.
- **S1** — Supõe-se que os grupos já usam Discord para conversar durante a sessão on-line.
