#pragma once

#include "../control/ControlePersonagem.h"
#include "../control/ControleUsuarios.h"
#include "../log/ILogger.h"
#include "ComandoComResultado.h"

// Comando do UC04 Excluir conta: exclui o usuario e os personagens dele
// (coordena dois controles). Resultado: false se o ID nao existir.
class ExcluirUsuarioComando : public ComandoComResultado<bool> {
public:
    ExcluirUsuarioComando(ControlePersonagem& personagens, ControleUsuarios& usuarios, int id,
                          ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    bool executarComResultado() override;

private:
    ControlePersonagem& personagens_;
    ControleUsuarios& usuarios_;
    int id_;
    ILogger& logger_;
};
