#include "DesfazerEdicaoPersonagemComando.h"

#include <string>

DesfazerEdicaoPersonagemComando::DesfazerEdicaoPersonagemComando(
    ControlePersonagem& controle, HistoricoEdicaoPersonagem& historico, ILogger& logger)
    : controle_(controle), historico_(historico), logger_(logger) {}

std::optional<Personagem> DesfazerEdicaoPersonagemComando::executarComResultado() {
    const std::optional<MementoPersonagem> memento = historico_.ultimo();
    if (!memento) {
        return std::nullopt;
    }
    // Se restaurar() lancar FalhaArmazenamentoException, o historico fica
    // como estava e o usuario pode tentar de novo.
    const std::optional<Personagem> restaurado = controle_.restaurar(*memento);
    historico_.descartar();

    const std::string id = std::to_string(memento->getIdPersonagem());
    if (restaurado) {
        logger_.info("Edicao desfeita: personagem ID " + id);
    } else {
        logger_.aviso("Edicao nao desfeita: personagem ID " + id + " nao existe mais");
    }
    return restaurado;
}

std::string DesfazerEdicaoPersonagemComando::descricaoFalha() const {
    return "Falha ao desfazer edicao de personagem";
}
