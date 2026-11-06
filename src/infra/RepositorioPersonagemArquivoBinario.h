#pragma once

#include <string>

#include "RepositorioPersonagemMemoria.h"

// Persiste os personagens em um arquivo binario (padrao: personagens.dat).
// Reaproveita as operacoes da versao em memoria e grava o arquivo inteiro a
// cada alteracao; se a gravacao falhar, a alteracao e desfeita na memoria.
//
// Formato (inteiros em little-endian):
//   int32 proximo ID, uint32 quantidade
//   para cada personagem:
//     int32 id, int32 id do dono, uint32 tamanho + bytes do nome,
//     int32 nivel, int32 pontos de vida,
//     int32 forca, destreza, constituicao, inteligencia, sabedoria, carisma
class RepositorioPersonagemArquivoBinario : public RepositorioPersonagemMemoria {
public:
    explicit RepositorioPersonagemArquivoBinario(std::string caminho = "personagens.dat");

    int adicionar(const Personagem& personagem) override;
    bool atualizar(const Personagem& personagem) override;
    bool remover(int id) override;

private:
    void carregar();
    void gravar() const;

    // Grava o arquivo; se falhar, volta ao estado `anterior` e relanca.
    void persistirOuDesfazer(const Estado& anterior);

    std::string caminho_;
};
