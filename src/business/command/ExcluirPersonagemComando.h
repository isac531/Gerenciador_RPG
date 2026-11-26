#pragma once

#include "../control/ControlePersonagem.h"
#include "../log/ILogger.h"
#include "ComandoComResultado.h"

// Comando do UC14 Excluir personagem. Resultado: false se o ID nao existir.
class ExcluirPersonagemComando : public ComandoComResultado<bool> {
public:
    ExcluirPersonagemComando(ControlePersonagem& controle, int id, ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    bool executarComResultado() override;

private:
    ControlePersonagem& controle_;
    int id_;
    ILogger& logger_;
};
