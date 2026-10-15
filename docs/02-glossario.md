# Glossário

> Seção 3 do modelo. Ordem alfabética. Estes termos são usados exatamente assim nas specs, nos casos de uso e no código (classes, atributos).
> As regras de jogo seguem o SRD 5.1 de D&D 5ª edição.

| Termo | Definição | Sinônimos a evitar | Exemplo |
|---|---|---|---|
| **Armazenamento** | Mecanismo em que o sistema guarda os usuários durante a execução, escolhido ao iniciar: *memória* (dados perdidos ao fechar) ou *arquivo binário* (dados mantidos entre execuções). | persistência (no texto para o usuário), banco | "Armazenamento: arquivo binário `usuarios.dat`" |
| **Atributo** | Uma das seis medidas básicas de um personagem: Força, Destreza, Constituição, Inteligência, Sabedoria e Carisma. Valor inteiro, normalmente de 1 a 20. | característica, stat | Força 16 |
| **Bônus de proficiência** | Valor somado a testes em que o personagem é treinado; depende do nível (+2 do nível 1 ao 4, até +6 no nível 17 em diante). | bônus de treino | Nível 5 → +3 |
| **Campanha** | Conjunto de sessões de jogo conduzidas por um Mestre, com uma história contínua e um número máximo de Jogadores. | mesa, aventura, partida | "A Mina Perdida de Phandelver", até 5 jogadores |
| **Classe de armadura (CA)** | Valor que uma rolagem de ataque precisa igualar ou superar para acertar o personagem. | defesa, armadura | CA 15 |
| **Classe de dificuldade (CD)** | Valor que um teste precisa igualar ou superar para ter sucesso; definido pelo Mestre. | dificuldade, alvo | CD 12 para escalar o muro |
| **Dado** | Dado de jogo identificado pela letra D e pelo número de faces. Tipos aceitos: D4, D6, D8, D10, D12, D20 e D100. | — | D20 |
| **Escudo do Mestre** | Área do sistema visível apenas ao Mestre, com anotações, NPCs e rolagens ocultas da campanha. | painel do mestre | — |
| **Ficha** | Conjunto de dados de um personagem: nome, nível, atributos, pontos de vida e demais valores de jogo. | planilha, sheet | — |
| **Jogador** | Papel do usuário que cria personagens e participa de campanhas. | player, participante | — |
| **Login** | Identificador do usuário para entrar no sistema: não vazio, no máximo 12 caracteres e sem números (RF02). | nome de usuário, username, nick | `isac` |
| **Mestre** | Papel do usuário que cria e conduz campanhas, controla NPCs e define testes. | narrador, GM, DM | — |
| **Modificador** | Valor derivado de um atributo e somado às rolagens: (atributo − 10) ÷ 2, arredondado para baixo. | bônus de atributo, mod | Força 16 → +3; Destreza 9 → −1 |
| **NPC** | Personagem controlado pelo Mestre, não por um Jogador (*non-player character*). | PdM, figurante | O taverneiro da vila |
| **Papel** | Tipo de usuário, que define o que ele pode fazer no sistema: Mestre ou Jogador. | perfil, tipo de conta | — |
| **Participação** | Vínculo entre um personagem e uma campanha, com data de entrada e status (ativa, saiu). | inscrição, matrícula | — |
| **Personagem** | Figura fictícia controlada por um Jogador dentro de uma campanha, descrita por uma ficha. | PJ, boneco, char | Thorin, anão guerreiro nível 3 |
| **Pontos de vida (PV)** | Quantidade de dano que um personagem aguenta; ao chegar a 0, ele fica inconsciente. | vida, HP, saúde | 27 PV |
| **Rolagem** | Lançamento de um ou mais dados de um mesmo tipo somado a um modificador, escrito *NdX+M*. O sistema registra o resultado, o autor e a data e hora. | jogada, roll | 2D6+3 = 11 |
| **Senha** | Segredo usado com o login para autenticar o usuário. Segue a política padrão do AWS IAM (RF03) e nunca é exibida. | password, código | — |
| **Teste** | Rolagem de D20 somada a um modificador (e ao bônus de proficiência, quando cabe) e comparada com uma classe de dificuldade. | check, verificação | Teste de Destreza: D20+2 contra CD 12 |
| **Usuário** | Pessoa cadastrada no sistema, com ID, login, senha e um papel. | conta, pessoa, membro | ID 1, login `isac`, papel Mestre |
| **Visitante** | Pessoa que usa o sistema sem estar autenticada; só pode criar uma conta. | anônimo, convidado | — |
