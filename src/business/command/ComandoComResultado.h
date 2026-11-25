#pragma once

#include <optional>
#include <stdexcept>

#include "IComando.h"

// Base dos comandos que devolvem um valor (ID criado, lista, usuario, ...).
// A subclasse implementa executarComResultado(); o valor fica guardado no
// comando e e lido com resultado() depois de executar().
template <typename Resultado>
class ComandoComResultado : public IComando {
public:
    void executar() final { resultado_.emplace(executarComResultado()); }

    // Lanca std::logic_error se o comando ainda nao foi executado
    // (ou terminou com excecao).
    const Resultado& resultado() const {
        if (!resultado_) {
            throw std::logic_error("Resultado do comando lido antes de executar().");
        }
        return *resultado_;
    }

protected:
    virtual Resultado executarComResultado() = 0;

private:
    std::optional<Resultado> resultado_;
};
