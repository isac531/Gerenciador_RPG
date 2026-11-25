#pragma once

#include "../log/ILogger.h"
#include "IComando.h"

// Invoker do padrao Command (ADR-0008): executa o comando recebido sem
// conhecer o caso de uso. Concentra o tratamento que vale para todos eles:
// se o armazenamento falhar, registra o erro no log e relanca a excecao.
class InvocadorComandos {
public:
    // Executa o comando. FalhaArmazenamentoException e registrada em
    // `logger` (com comando.descricaoFalha()) e relancada; qualquer outra
    // excecao passa sem alteracao.
    void executar(IComando& comando, ILogger& logger) const;
};
