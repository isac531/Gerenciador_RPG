#include <iostream>
#include <memory>

#include "business/control/FacadeSingletonController.h"
#include "business/exception/Excecoes.h"
#include "infra/EscolhaFabrica.h"
#include "ui/MenuPrincipal.h"

namespace {

// Se algum arquivo binario nao puder ser carregado, informa e usa a memoria (RAM).
Repositorios montarRepositorios(TipoArmazenamento tipo) {
    if (tipo == TipoArmazenamento::MEMORIA) {
        std::cout << "Usando armazenamento em memoria (RAM).\n";
        return escolherFabrica(TipoArmazenamento::MEMORIA)->criarRepositorios();
    }

    try {
        auto repositorios = escolherFabrica(TipoArmazenamento::ARQUIVO_BINARIO)->criarRepositorios();
        std::cout << "Usando arquivos binarios usuarios.dat, personagens.dat e acessos.dat.\n";
        return repositorios;
    } catch (const FalhaArmazenamentoException& e) {
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

    const Repositorios repositorios = montarRepositorios(*tipo);
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    fachada.inicializar(*repositorios.usuarios, *repositorios.personagens,
                        *repositorios.acessos);

    MenuPrincipal(fachada).executar();
    return 0;
}
