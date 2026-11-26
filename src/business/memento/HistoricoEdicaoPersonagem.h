#pragma once

#include <optional>

#include "MementoPersonagem.h"

// Caretaker do padrao Memento (ADR-0009): guarda o memento da ultima
// edicao de ficha, sem ler o que ha dentro dele. So a ultima edicao pode
// ser desfeita (RF15): guardar um memento novo substitui o anterior.
// Vive so na memoria da execucao atual.
class HistoricoEdicaoPersonagem {
public:
    void guardar(const MementoPersonagem& memento);

    // Memento da ultima edicao, sem retira-lo; std::nullopt se nao ha.
    std::optional<MementoPersonagem> ultimo() const;

    bool vazio() const;

    // Esquece a ultima edicao (depois de desfeita, ou ao reinicializar).
    void descartar();

private:
    std::optional<MementoPersonagem> ultimo_;
};
