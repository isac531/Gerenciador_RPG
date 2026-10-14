#pragma once

#include <memory>
#include <vector>

#include "../exception/Excecoes.h"
#include "../model/Usuario.h"
#include "../repository/IUsuarioRepositorio.h"
#include "../validation/IRegraValidacao.h"

class ControleCriarConta {
public:
    // O repositorio e injetado: o controle nao sabe se os dados ficam
    // em memoria ou em arquivo.
    explicit ControleCriarConta(std::unique_ptr<IUsuarioRepositorio> repositorio);

    // Valida e cadastra o usuario.
    // Lanca LoginInvalidoException / SenhaInvalidaException (e subclasses),
    // std::invalid_argument (ID invalido ou ja existente) ou
    // FalhaArmazenamentoException (erro ao gravar).
    void criarConta(const Usuario& usuario);

    // Executam as regras de login / senha; lancam excecao se alguma for violada.
    void validaLogin(const Usuario& usuario) const;
    void validaSenha(const Usuario& usuario) const;

    // Retorna false se o ID nao existir. Pode lancar FalhaArmazenamentoException.
    bool excluirConta(int id);

    // Usuarios cadastrados em ordem crescente de ID.
    std::vector<Usuario> listar() const;

private:
    using Regras = std::vector<std::unique_ptr<IRegraValidacao>>;

    static void executar(const Regras& regras, const Usuario& usuario);

    std::unique_ptr<IUsuarioRepositorio> repositorio_;
    Regras regrasLogin_;
    Regras regrasSenha_;
};
