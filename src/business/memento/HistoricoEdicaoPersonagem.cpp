#include "HistoricoEdicaoPersonagem.h"

void HistoricoEdicaoPersonagem::guardar(const MementoPersonagem& memento) {
    ultimo_ = memento;
}

std::optional<MementoPersonagem> HistoricoEdicaoPersonagem::ultimo() const {
    return ultimo_;
}

bool HistoricoEdicaoPersonagem::vazio() const {
    return !ultimo_.has_value();
}

void HistoricoEdicaoPersonagem::descartar() {
    ultimo_.reset();
}
