#pragma once

#include "../control/ControlePersonagem.h"
#include "../log/ILogger.h"
#include "../model/Personagem.h"
#include "ComandoComResultado.h"

// Comando do UC07 Criar personagem. Resultado: o ID gerado.
class CriarPersonagemComando : public ComandoComResultado<int> {
public:
    CriarPersonagemComando(ControlePersonagem& controle, Personagem personagem, ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    int executarComResultado() override;

private:
    ControlePersonagem& controle_;
    Personagem personagem_;
    ILogger& logger_;
};
