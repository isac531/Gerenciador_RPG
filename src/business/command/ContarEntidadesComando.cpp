#include "ContarEntidadesComando.h"

ContarEntidadesComando::ContarEntidadesComando(const ControleUsuarios& usuarios,
                                               const ControlePersonagem& personagens)
    : usuarios_(usuarios), personagens_(personagens) {}

std::string ContarEntidadesComando::descricaoFalha() const {
    return "Falha ao contar entidades";
}

QuantidadeEntidades ContarEntidadesComando::executarComResultado() {
    QuantidadeEntidades quantidade;
    quantidade.usuarios = usuarios_.quantidade();
    quantidade.personagens = personagens_.quantidade();
    return quantidade;
}
