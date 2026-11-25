#pragma once

#include "../control/ControlePersonagem.h"
#include "../log/ILogger.h"
#include "../model/Ficha.h"
#include "IComando.h"

// Comando do UC12 Editar ficha. Nao devolve valor: se a ficha for invalida
// ou o personagem nao existir, executar() lanca a excecao do controle.
class EditarPersonagemComando : public IComando {
public:
    EditarPersonagemComando(ControlePersonagem& controle, int id, Ficha ficha, ILogger& logger);

    void executar() override;
    std::string descricaoFalha() const override;

private:
    ControlePersonagem& controle_;
    int id_;
    Ficha ficha_;
    ILogger& logger_;
};
