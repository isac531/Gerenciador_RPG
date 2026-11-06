#include <iostream>
#include <memory>

#include "business/control/FacadeSingletonController.h"
#include "business/exception/Excecoes.h"
#include "infra/FabricaRepositorio.h"
#include "ui/MenuPrincipal.h"

namespace {

struct Repositorios {
    std::unique_ptr<IUsuarioRepositorio> usuarios;
    std::unique_ptr<IPersonagemRepositorio> personagens;
};

Repositorios criarRepositorios(TipoArmazenamento tipo) {
    return {criarRepositorioUsuarios(tipo), criarRepositorioPersonagens(tipo)};
}

// Se algum arquivo binario nao puder ser carregado, informa e usa a memoria (RAM).
Repositorios montarRepositorios(TipoArmazenamento tipo) {
    if (tipo == TipoArmazenamento::MEMORIA) {
        std::cout << "Usando armazenamento em memoria (RAM).\n";
        return criarRepositorios(TipoArmazenamento::MEMORIA);
    }

    try {
        auto repositorios = criarRepositorios(TipoArmazenamento::ARQUIVO_BINARIO);
        std::cout << "Usando arquivos binarios usuarios.dat e personagens.dat.\n";
        return repositorios;
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n'
                  << "Usando armazenamento em memoria (RAM); "
                     "os dados nao serao salvos.\n";
        return criarRepositorios(TipoArmazenamento::MEMORIA);
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
    fachada.inicializar(*repositorios.usuarios, *repositorios.personagens);

    MenuPrincipal(fachada).executar();
    return 0;
}
