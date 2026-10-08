#pragma once

#include <map>
#include <memory>
#include <vector>

#include "../entity/Usuario.h"
#include "../exception/Excecoes.h"
#include "../validation/IRegraValidacao.h"

class ControleCriarConta {
public:
    ControleCriarConta();

    // Valida e cadastra o usuario.
    // Lanca LoginInvalidoException / SenhaInvalidaException (e subclasses)
    // ou std::invalid_argument (ID invalido ou ja existente).
    void criarConta(const Usuario& usuario);

    // Executam as regras de login / senha; lancam excecao se alguma for violada.
    void validaLogin(const Usuario& usuario) const;
    void validaSenha(const Usuario& usuario) const;

    // Operacoes extras mantidas do codigo original.
    bool excluirConta(int id);
    const std::map<int, Usuario>& listar() const;

private:
    using Regras = std::vector<std::unique_ptr<IRegraValidacao>>;

    static void executar(const Regras& regras, const Usuario& usuario);
    void carregarUsuarios();
    void salvarUsuarios() const;

    std::map<int, Usuario> usuarios_;
    Regras regrasLogin_;
    Regras regrasSenha_;
};
