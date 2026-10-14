#include <iostream>
#include <memory>

#include "business/control/ControleCriarConta.h"
#include "business/control/ControleUsuarios.h"
#include "business/exception/Excecoes.h"
#include "infra/FabricaRepositorio.h"
#include "ui/MenuPrincipal.h"

// Raiz de composicao: unico ponto que conhece ui, business e infra juntos.
// Se o arquivo binario nao puder ser carregado, informa e usa a memoria (RAM).
static std::unique_ptr<IUsuarioRepositorio> montarRepositorio(TipoArmazenamento tipo) {
    if (tipo == TipoArmazenamento::MEMORIA) {
        std::cout << "Usando armazenamento em memoria (RAM).\n";
        return criarRepositorio(TipoArmazenamento::MEMORIA);
    }

    try {
        auto repositorio = criarRepositorio(TipoArmazenamento::ARQUIVO_BINARIO);
        std::cout << "Usando arquivo binario usuarios.dat.\n";
        return repositorio;
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n'
                  << "Usando armazenamento em memoria (RAM); "
                     "os dados nao serao salvos.\n";
        return criarRepositorio(TipoArmazenamento::MEMORIA);
    }
}

int main() {
    const auto tipo = MenuPrincipal::escolherArmazenamento();
    if (!tipo) {
        return 0;  // Entrada encerrada antes da escolha.
    }

    const auto repositorio = montarRepositorio(*tipo);
    ControleCriarConta controleCriarConta(*repositorio);
    ControleUsuarios controleUsuarios(*repositorio);
    MenuPrincipal menu(controleCriarConta, controleUsuarios);
    menu.executar();
    return 0;
}
