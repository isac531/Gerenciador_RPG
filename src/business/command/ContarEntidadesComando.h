#pragma once

#include <cstddef>

#include "../control/ControlePersonagem.h"
#include "../control/ControleUsuarios.h"
#include "ComandoComResultado.h"

// Quantidade de entidades cadastradas (RF12).
struct QuantidadeEntidades {
    std::size_t usuarios = 0;
    std::size_t personagens = 0;

    std::size_t total() const { return usuarios + personagens; }
};

// Comando do UC15 Consultar quantidade de entidades (coordena dois controles).
class ContarEntidadesComando : public ComandoComResultado<QuantidadeEntidades> {
public:
    ContarEntidadesComando(const ControleUsuarios& usuarios, const ControlePersonagem& personagens);

    std::string descricaoFalha() const override;

protected:
    QuantidadeEntidades executarComResultado() override;

private:
    const ControleUsuarios& usuarios_;
    const ControlePersonagem& personagens_;
};
