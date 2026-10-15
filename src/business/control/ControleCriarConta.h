#pragma once

#include <memory>
#include <vector>

#include "../exception/Excecoes.h"
#include "../model/Usuario.h"
#include "../repository/IUsuarioRepositorio.h"
#include "../validation/IRegraValidacao.h"

// Controle do caso de uso UC01 Criar conta: valida os dados e guarda o usuario.
class ControleCriarConta {
public:
    // O repositorio e injetado: o controle nao sabe se os dados ficam
    // em memoria ou em arquivo.
    explicit ControleCriarConta(IUsuarioRepositorio& repositorio);

    // Valida e cadastra o usuario.
    // Lanca IdInvalidoException, LoginInvalidoException e SenhaInvalidaException
    // (e subclasses) ou FalhaArmazenamentoException (erro ao gravar).
    void criarConta(const Usuario& usuario);

    // Executam as regras de login / senha; lancam excecao se alguma for violada.
    void validaLogin(const Usuario& usuario) const;
    void validaSenha(const Usuario& usuario) const;

private:
    using Regras = std::vector<std::unique_ptr<IRegraValidacao>>;

    static void executar(const Regras& regras, const Usuario& usuario);
    void validaId(const Usuario& usuario) const;

    IUsuarioRepositorio& repositorio_;
    Regras regrasLogin_;
    Regras regrasSenha_;
};
