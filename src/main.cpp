#include <iostream>
#include <memory>
#include <string>

#include "business/control/FacadeSingletonController.h"
#include "business/exception/Excecoes.h"
#include "infra/EscolhaFabrica.h"
#include "infra/LogCAdapter.h"
#include "ui/MenuPrincipal.h"

namespace {

// Se algum arquivo binario nao puder ser carregado, informa e usa a memoria (RAM).
Repositorios montarRepositorios(TipoArmazenamento tipo, ILogger& logger) {
    if (tipo == TipoArmazenamento::MEMORIA) {
        std::cout << "Usando armazenamento em memoria (RAM).\n";
        logger.info("Armazenamento escolhido: memoria");
        return escolherFabrica(TipoArmazenamento::MEMORIA)->criarRepositorios();
    }

    try {
        auto repositorios = escolherFabrica(TipoArmazenamento::ARQUIVO_BINARIO)->criarRepositorios();
        std::cout << "Usando arquivos binarios usuarios.dat, personagens.dat e acessos.dat.\n";
        logger.info("Armazenamento escolhido: arquivo binario");
        return repositorios;
    } catch (const FalhaArmazenamentoException& e) {
        logger.erro(std::string("Falha ao carregar os arquivos, usando memoria: ") + e.what());
        std::cout << "Falha no armazenamento: " << e.what() << '\n'
                  << "Usando armazenamento em memoria (RAM); "
                     "os dados nao serao salvos.\n";
        return escolherFabrica(TipoArmazenamento::MEMORIA)->criarRepositorios();
    }
}

}  // namespace

// Raiz de composicao: unico ponto que conhece ui, business e infra juntos.
int main() {
    const auto tipo = MenuPrincipal::escolherArmazenamento();
    if (!tipo) {
        return 0;  // Entrada encerrada antes da escolha.
    }

    // Adapter: a business so conhece ILogger; a biblioteca log.c fica na infra.
    LogCAdapter logger("rpg.log");
    logger.info("Sistema iniciado");

    const Repositorios repositorios = montarRepositorios(*tipo, logger);
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    fachada.inicializar(*repositorios.usuarios, *repositorios.personagens,
                        *repositorios.acessos);
    fachada.definirLogger(logger);

    MenuPrincipal(fachada).executar();
    logger.info("Sistema encerrado");
    return 0;
}
