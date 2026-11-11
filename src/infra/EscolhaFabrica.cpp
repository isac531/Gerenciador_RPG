#include "EscolhaFabrica.h"

#include "FabricaArquivoBinario.h"
#include "FabricaMemoria.h"

std::unique_ptr<FabricaRepositorio> escolherFabrica(TipoArmazenamento tipo) {
    if (tipo == TipoArmazenamento::ARQUIVO_BINARIO) {
        return std::make_unique<FabricaArquivoBinario>();
    }
    return std::make_unique<FabricaMemoria>();
}
