#pragma once

#include <vector>

#include "../control/ControleUsuarios.h"
#include "../model/Usuario.h"
#include "ComandoComResultado.h"

// Comando do UC03 Listar usuarios. Resultado: usuarios em ordem de ID.
class ListarUsuariosComando : public ComandoComResultado<std::vector<Usuario>> {
public:
    explicit ListarUsuariosComando(const ControleUsuarios& controle);

    std::string descricaoFalha() const override;

protected:
    std::vector<Usuario> executarComResultado() override;

private:
    const ControleUsuarios& controle_;
};
