#include "InvocadorComandos.h"

#include "../exception/Excecoes.h"

void InvocadorComandos::executar(IComando& comando, ILogger& logger) const {
    try {
        comando.executar();
    } catch (const FalhaArmazenamentoException& e) {
        logger.erro(comando.descricaoFalha() + ": " + e.what());
        throw;
    }
}
