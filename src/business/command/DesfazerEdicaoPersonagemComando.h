#pragma once

#include <optional>

#include "../control/ControlePersonagem.h"
#include "../log/ILogger.h"
#include "../memento/HistoricoEdicaoPersonagem.h"
#include "../model/Personagem.h"
#include "ComandoComResultado.h"

// Comando do UC17 Desfazer edicao da ficha (RF15, Memento, ADR-0009).
// Le o memento da ultima edicao no historico (Caretaker) e pede ao
// ControlePersonagem que volte o personagem a ele. Resultado: o personagem
// restaurado, ou std::nullopt se nao ha edicao para desfazer (nenhuma
// edicao, ja desfeita, ou personagem excluido depois da edicao).
// Se a gravacao falhar, o memento continua no historico.
class DesfazerEdicaoPersonagemComando : public ComandoComResultado<std::optional<Personagem>> {
public:
    DesfazerEdicaoPersonagemComando(ControlePersonagem& controle,
                                    HistoricoEdicaoPersonagem& historico, ILogger& logger);

    std::string descricaoFalha() const override;

protected:
    std::optional<Personagem> executarComResultado() override;

private:
    ControlePersonagem& controle_;
    HistoricoEdicaoPersonagem& historico_;
    ILogger& logger_;
};
