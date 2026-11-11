#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "../model/Usuario.h"
#include "../repository/IAcessoRepositorio.h"
#include "../repository/IUsuarioRepositorio.h"

// Controle do caso de uso UC02 Fazer login (RF07) e do registro de acessos
// (RF13). Toda tentativa, aceita ou recusada, vira um RegistroAcesso.
class ControleLogin {
public:
    // Devolve o instante atual em segundos desde 1970 (UTC).
    using Relogio = std::function<std::int64_t()>;

    // Relogio padrao: hora do sistema.
    static std::int64_t agora();

    // Os repositorios sao injetados; o relogio tambem, para os testes.
    ControleLogin(IUsuarioRepositorio& usuarios, IAcessoRepositorio& acessos,
                  Relogio relogio = agora);

    // Retorna o usuario autenticado. Lanca CredenciaisInvalidasException se
    // o login nao existir ou a senha nao conferir (mesma mensagem nos dois
    // casos, NF-SEG-02) e FalhaArmazenamentoException se o registro do
    // acesso nao puder ser gravado.
    Usuario autenticar(const std::string& login, const std::string& senha);

private:
    IUsuarioRepositorio& usuarios_;
    IAcessoRepositorio& acessos_;
    Relogio relogio_;
};
