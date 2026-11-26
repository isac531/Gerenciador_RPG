#pragma once

#include "../control/ControlePersonagem.h"
#include "../log/ILogger.h"
#include "../memento/HistoricoEdicaoPersonagem.h"
#include "../model/Ficha.h"
#include "IComando.h"

// Comando do UC12 Editar ficha. Nao devolve valor: se a ficha for invalida
// ou o personagem nao existir, executar() lanca a excecao do controle.
// Depois de a edicao ser aceita e gravada, guarda no historico o memento do
// estado anterior, para o UC17 Desfazer edicao (Memento, ADR-0009). Uma
// edicao recusada nao mexe no historico.
class EditarPersonagemComando : public IComando {
public:
    EditarPersonagemComando(ControlePersonagem& controle, HistoricoEdicaoPersonagem& historico,
                            int id, Ficha ficha, ILogger& logger);

    void executar() override;
    std::string descricaoFalha() const override;

private:
    ControlePersonagem& controle_;
    HistoricoEdicaoPersonagem& historico_;
    int id_;
    Ficha ficha_;
    ILogger& logger_;
};
