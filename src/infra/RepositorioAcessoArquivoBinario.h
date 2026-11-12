#pragma once

#include <string>

#include "RepositorioAcessoMemoria.h"

// Persiste os registros de acesso em um arquivo binario (padrao: acessos.dat).
// Reaproveita a versao em memoria e grava o arquivo inteiro a cada registro;
// se a gravacao falhar, o registro e desfeito na memoria.
//
// Formato (inteiros em little-endian):
//   int32 proximo ID, uint32 quantidade
//   para cada registro:
//     int32 id, uint32 tamanho + bytes do login, int32 id do usuario,
//     int64 data e hora (segundos desde 1970, UTC), uint8 sucesso (0 ou 1)
class RepositorioAcessoArquivoBinario : public RepositorioAcessoMemoria {
public:
    explicit RepositorioAcessoArquivoBinario(std::string caminho = "acessos.dat");

    int adicionar(const RegistroAcesso& registro) override;

private:
    void carregar();
    void gravar() const;

    std::string caminho_;
};
