#include <utility>

#include "business/control/ControleCriarConta.h"
#include "ui/MenuPrincipal.h"

int main() {
    auto repositorio = MenuPrincipal::escolherArmazenamento();
    if (!repositorio) {
        return 0;  // Entrada encerrada antes da escolha.
    }

    ControleCriarConta controle(std::move(repositorio));
    MenuPrincipal menu(controle);
    menu.executar();
    return 0;
}
