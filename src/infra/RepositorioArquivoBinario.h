#pragma once

#include <map>
#include <string>

#include "../business/repository/IUsuarioRepositorio.h"

// Persiste os usuarios em um arquivo binario (padrao: usuarios.dat).
//
// Formato (inteiros em little-endian):
//   uint32 quantidade
//   para cada usuario:
//     int32  id
//     uint32 tamanho do login + bytes do login
//     uint32 tamanho da senha + bytes da senha
//     uint8  papel (1 = Mestre, 2 = Jogador)
//
// O arquivo e lido no construtor e reescrito a cada alteracao. Qualquer
// falha de E/S ou arquivo corrompido gera FalhaArmazenamentoException; se a
// gravacao falhar, a alteracao e desfeita tambem na memoria.
class RepositorioArquivoBinario : public IUsuarioRepositorio {
public:
    explicit RepositorioArquivoBinario(std::string caminho = "usuarios.dat");

    bool adicionar(const Usuario& usuario) override;
    bool remover(int id) override;
    bool existe(int id) const override;
    std::optional<Usuario> buscar(int id) const override;
    std::vector<Usuario> listarTodos() const override;

    const std::string& getCaminho() const;

private:
    void carregar();
    void gravar() const;

    std::string caminho_;
    std::map<int, Usuario> usuarios_;
};
