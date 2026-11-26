#pragma once

#include <vector>

#include "../control/ControlePersonagem.h"
#include "../model/Personagem.h"
#include "ComandoComResultado.h"

// Comando do UC13 Listar personagens. Resultado: personagens em ordem de ID.
class ListarPersonagensComando : public ComandoComResultado<std::vector<Personagem>> {
public:
    explicit ListarPersonagensComando(const ControlePersonagem& controle);

    std::string descricaoFalha() const override;

protected:
    std::vector<Personagem> executarComResultado() override;

private:
    const ControlePersonagem& controle_;
};
