#pragma once

#include <optional>

#include "../control/ControlePersonagem.h"
#include "../model/Personagem.h"
#include "ComandoComResultado.h"

// Comando de consulta de um personagem (usado pelo UC12 para mostrar a
// ficha atual). Resultado: o personagem, ou std::nullopt se o ID nao existir.
class BuscarPersonagemComando : public ComandoComResultado<std::optional<Personagem>> {
public:
    BuscarPersonagemComando(const ControlePersonagem& controle, int id);

    std::string descricaoFalha() const override;

protected:
    std::optional<Personagem> executarComResultado() override;

private:
    const ControlePersonagem& controle_;
    int id_;
};
