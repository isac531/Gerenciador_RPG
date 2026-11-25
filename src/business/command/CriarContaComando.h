#pragma once

#include "../control/ControleCriarConta.h"
#include "../log/ILogger.h"
#include "../model/Usuario.h"
#include "ComandoComResultado.h"

// Comando do UC01 Criar conta. Resultado: o ID gerado para o usuario.
class CriarContaComando : public ComandoComResultado<int> {
public:
    CriarContaComando(ControleCriarConta& controle, Usuario usuario, ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    int executarComResultado() override;

private:
    ControleCriarConta& controle_;
    Usuario usuario_;
    ILogger& logger_;
};
