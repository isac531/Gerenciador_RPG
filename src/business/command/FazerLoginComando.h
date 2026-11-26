#pragma once

#include <string>

#include "../control/ControleLogin.h"
#include "../log/ILogger.h"
#include "../model/Usuario.h"
#include "ComandoComResultado.h"

// Comando do UC02 Fazer login (registra o acesso, RF13).
// Resultado: o usuario autenticado.
class FazerLoginComando : public ComandoComResultado<Usuario> {
public:
    FazerLoginComando(ControleLogin& controle, std::string login, std::string senha,
                      ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    Usuario executarComResultado() override;

private:
    ControleLogin& controle_;
    std::string login_;
    std::string senha_;
    ILogger& logger_;
};
