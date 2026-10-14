# Elicitação de requisitos

> Seção 4 do modelo. Para cada técnica: **por que** foi escolhida, **quando, onde e com quem** foi aplicada, e o que se descobriu.
> Anexos (roteiros, instrumentos, fontes) ficam em [`03-elicitacao/anexos/`](03-elicitacao/anexos/).

Nesta primeira iteração foram usadas duas técnicas que não dependem da agenda de terceiros e se apoiam em fontes verificáveis: a **análise de sistemas similares** (benchmarking), para entender o que as ferramentas existentes oferecem, e a **análise de documentos** com relatos públicos de usuários dessas ferramentas, para saber o que os usuários reais reclamam. Um **questionário** com jogadores e mestres foi preparado para a próxima iteração (§4.3).

## 4.1 Análise de sistemas similares (benchmarking)

| Campo | Registro |
|---|---|
| Data | 14/10/2026 |
| Local | Remoto: páginas oficiais, documentação e verbetes públicos das ferramentas |
| Participantes | Equipe do projeto (ver README), com apoio de assistente de IA na coleta e no resumo das fontes |
| Sistemas analisados | D&D Beyond, Roll20, Foundry VTT, Owlbear Rodeo |
| Instrumento e resultados | [`anexos/benchmarking-ferramentas.md`](03-elicitacao/anexos/benchmarking-ferramentas.md) |

**Por que esta técnica:** o projeto nasceu da insatisfação do grupo com uma ferramenta existente (D&D Beyond). Comparar essa ferramenta com as concorrentes mostra quais funções são indispensáveis num gerenciador de campanha, qual é o modelo de custo de cada uma e onde há espaço para uma solução mais simples. É barata, rápida e não depende de terceiros.

**Procedimento:** cada ferramenta foi comparada pelos mesmos critérios: foco do produto, cálculo automático da ficha, gerenciamento de campanha, rolagem integrada, presença de mapas, modelo de custo e presença de loja ou anúncios. Cada célula da tabela do anexo cita a fonte.

**Principais achados:**
- O **D&D Beyond** (da Wizards of the Coast desde 2022) reúne construtor de personagem com ficha digital, rolador de dados, compêndio e loja de livros digitais. A conta gratuita mostra anúncios e limita a 6 personagens; a assinatura Hero remove os anúncios, e a Master permite compartilhar conteúdo com até 3 campanhas de 12 jogadores. O conteúdo dos livros é vendido à parte. Ou seja, ficha automática e campanha existem, mas misturadas com venda de conteúdo (→ P1, escopo negativo "loja" e "anúncios").
- **Roll20** e **Foundry VTT** são mesas virtuais completas, centradas em mapas e tokens. O Roll20 tem versão gratuita limitada e assinaturas mensais; o Foundry cobra licença única (US$ 50, pelo mestre) e exige hospedar o servidor no próprio computador (com redirecionamento de portas) ou contratar hospedagem, e é recomendado para mestres com conhecimento técnico (→ P2, P5, escopo negativo "mapas", R5).
- O **Owlbear Rodeo** é uma mesa virtual gratuita e minimalista, focada em mapa e rolagem, que dispensa cadastro e é elogiada pela facilidade; em troca, as fontes não citam fichas nem cálculo automático, e o conteúdo fica salvo só no navegador (→ oportunidade: simplicidade com automação).
- As ferramentas que guardam fichas e campanhas (D&D Beyond, Roll20, Foundry VTT) exigem conta com login e senha e separam o papel de Mestre do papel de Jogador (→ RF01, RF04, papéis Mestre e Jogador).
- Em todas, a rolagem é feita na plataforma e o resultado fica visível para o grupo, recurso que o sistema deve manter (→ P4, visão §2.3).

## 4.2 Análise de documentos: relatos públicos de usuários

| Campo | Registro |
|---|---|
| Data | 14/10/2026 |
| Local | Remoto: fórum oficial de feedback do D&D Beyond, estudo de usabilidade publicado sobre o Roll20, avaliações e análises de ferramentas |
| Participantes | Equipe do projeto (ver README), com apoio de assistente de IA na coleta e no resumo das fontes |
| Documentos analisados | 3 tópicos do fórum oficial do D&D Beyond (2020–2026), a seção de críticas do verbete "D&D Beyond" da Wikipedia, 1 teste de usabilidade do Roll20 com 4 participantes, 2 páginas de avaliação (Roll20 e Foundry VTT); lista completa no anexo |
| Instrumento e resultados | [`anexos/analise-relatos-usuarios.md`](03-elicitacao/anexos/analise-relatos-usuarios.md) |

**Por que esta técnica:** a equipe não teve tempo hábil para recrutar participantes nesta iteração, mas milhares de usuários das ferramentas concorrentes já descreveram publicamente seus problemas. Ler esses relatos de forma sistemática (registrando fonte, data e problema citado) dá evidência real, datada e verificável sobre as dores dos usuários, e complementa o benchmarking, que só mostra o que as ferramentas *oferecem*.

**Procedimento:** os documentos foram lidos e cada reclamação ou elogio recorrente foi classificado em uma categoria (anúncios e loja, navegação e interface, custo, curva de aprendizado, automação). Trechos literais e links estão no anexo.

**Principais achados:**
- **Anúncios incomodam até quem paga.** No tópico "Love D&D Beyond, but please enough with the advertisement" (out/2020), usuários relatam que o banner de promoção cobre itens do menu durante a sessão e pedem para "jogar sem ter isso empurrado o tempo todo"; um moderador da empresa reconheceu como "boa sugestão" uma opção para desligar os banners. No tópico "Ads!" (mar/2023), um assinante do nível mais caro reclama de ver anúncios da própria assinatura que já paga (→ P1, escopo negativo "anúncios").
- **A página inicial prioriza vendas.** No tópico "New Beyond Webpage is Terrible" (dez/2025 a mar/2026), usuários dizem que o topo da página é ocupado por propaganda, inclusive de conteúdo que já possuem, e que "tudo abaixo é basicamente anúncio para vender mais" (→ P1, P2).
- **A interface do Roll20 é difícil para iniciantes.** No teste de usabilidade publicado por Nina Oria-Loureiro (Bentley University), os participantes iniciantes não encontraram o construtor de personagem, cujo nome não indica a função; ficaram mais frustrados do que satisfeitos; e não gostaram de alternar entre janelas e abas para ver o resultado dos testes. Todos disseram que mestrariam um jogo se a barreira de entrada fosse menor (→ P2, NF-USA-01, NF-USA-02).
- **Recursos de grupo atrás de pagamento.** Avaliações do Roll20 citam funções importantes bloqueadas para assinantes; no Foundry VTT, o custo da licença e a hospedagem própria são citados como barreira para mestres sem conhecimento técnico (→ P5, R5).

## 4.3 Considerações finais

As duas técnicas se complementam: o benchmarking mostrou o que já existe e como cada ferramenta cobra por isso, e a análise dos relatos mostrou, com a voz dos próprios usuários, que anúncios, loja e interfaces complexas atrapalham o uso. Juntas, elas definiram o escopo (campanhas, fichas, dados e cálculos, sem loja, anúncios ou mapas) e o núcleo inicial: gerenciamento de usuários com os papéis Mestre e Jogador.

**Limitações:** as duas técnicas usam fontes secundárias, sobre ferramentas concorrentes. Os relatos de fórum vêm de usuários que se deram ao trabalho de reclamar, o que pode exagerar os problemas. Para a próxima iteração, antes de especificar campanhas, fichas e rolagens, está planejado um **questionário com jogadores e mestres** ([`anexos/questionario-jogadores.md`](03-elicitacao/anexos/questionario-jogadores.md)) para medir a frequência desses problemas no nosso público, e uma entrevista com um mestre experiente sobre o Escudo do Mestre e os NPCs.

### Rastreabilidade problema → requisito

| Problema | Origem | Requisitos e decisões |
|---|---|---|
| P1 Anúncios e loja | 4.1, 4.2 | Escopo negativo (loja, anúncios) |
| P2 Interface complexa | 4.1, 4.2 | NF-USA-01, NF-USA-02, NF-USA-03, escopo negativo (mapas) |
| P3 Contas manuais | Relato da equipe; 4.1 (todas as ferramentas pagas automatizam a ficha) | Visão §2.3 (cálculos automáticos; requisitos nas próximas entregas) |
| P4 Rolagens sem registro | 4.1 | Visão §2.3 (rolagem com histórico) |
| P5 Custo e infraestrutura | 4.1, 4.2 | R5, NF-POR-01, RF05 (armazenamento local, sem servidor) |
| Todas exigem conta e separam Mestre de Jogador | 4.1 | RF01, RF02, RF03, RF04, RF06, RF07, NF-SEG-01 |
