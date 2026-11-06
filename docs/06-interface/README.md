# Descrição da interface com o usuário

> Seção 8 do modelo. Nesta fase a interface é de **console** (restrição R1). Cada tela tem um ID (TLnn) citado nos casos de uso.
> Os wireframes abaixo mostram as telas de console dos três casos de uso detalhados (UC01, UC07 e UC12) e das demais funções da Sprint 3, com uma entrada de exemplo.

## TL01 — Escolha do armazenamento

Relacionada a: todos os casos de uso · RF05, NF-MAN-01

```text
Escolha o armazenamento:
1. Memoria (RAM)
2. Arquivo binario (usuarios.dat e personagens.dat)
Opcao: 2
Usando arquivos binarios usuarios.dat e personagens.dat.
```

## TL02 — Menu principal e cadastro de usuário

Relacionada a: UC01, UC03, UC04, UC15 · RF01–RF04, RF06, RF12, NF-USA-01, NF-USA-03

```text
1. Cadastrar usuario
2. Excluir usuario
3. Listar usuarios
4. Personagens
5. Quantidade de entidades cadastradas
0. Sair
Opcao: 1
Login: isac
Senha: Dado#2026
Papel (1. Mestre  2. Jogador): 1
Usuario cadastrado com sucesso. ID: 1

Opcao: 3
ID: 1 | Login: isac | Papel: Mestre

Opcao: 5
Entidades cadastradas: 2 (usuarios: 1, personagens: 1)

Opcao: 2
ID do usuario: 1
Usuario e seus personagens excluidos com sucesso.
```

## TL03 — Menu de personagens e listagem

Relacionada a: UC07, UC12, UC13, UC14 · RF08–RF11

```text
Personagens
1. Criar personagem
2. Listar personagens
3. Editar ficha
4. Excluir personagem
0. Voltar
Opcao: 2
ID: 1 | Dono: isac | Nome: Thorin | Nivel: 3 | PV: 27
    FOR 16 (+3) DES 12 (+1) CON 14 (+2) INT 8 (-1) SAB 10 (+0) CAR 9 (-1)

Opcao: 4
ID do personagem: 9
Personagem nao encontrado.
```

## TL04 — Criar personagem

Relacionada a: UC07 · RF08, NF-USA-01

```text
Opcao: 1
ID do usuario dono: 1
Nome: Thorin
Nivel (1 a 20): 3
Pontos de vida: 27
FOR (1 a 30): 16
DES (1 a 30): 12
CON (1 a 30): 14
INT (1 a 30): 8
SAB (1 a 30): 10
CAR (1 a 30): 9
Personagem criado com sucesso. ID: 1
```

Recusas: `Nao foi possivel criar: Nao existe usuario com esse ID para ser o dono.` · `Ficha invalida: O nivel deve estar entre 1 e 20.` · `Valor invalido.`

## TL05 — Editar ficha

Relacionada a: UC12 · RF10, NF-USA-01

```text
Opcao: 3
ID do personagem: 1
Ficha atual:
ID: 1 | Dono: ID 1 | Nome: Thorin | Nivel: 3 | PV: 27
    FOR 16 (+3) DES 12 (+1) CON 14 (+2) INT 8 (-1) SAB 10 (+0) CAR 9 (-1)
Nome: Thorin
Nivel (1 a 20): 4
Pontos de vida: 35
FOR (1 a 30): 16
DES (1 a 30): 12
CON (1 a 30): 14
INT (1 a 30): 8
SAB (1 a 30): 10
CAR (1 a 30): 9
Ficha atualizada com sucesso.
```
