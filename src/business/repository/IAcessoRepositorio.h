#pragma once

#include <cstddef>
#include <vector>

#include "../model/RegistroAcesso.h"

// Contrato de armazenamento dos registros de acesso (padrao Repository,
// ADR-0005). O historico so cresce: nao ha alterar nem remover, porque um
// relatorio de acessos precisa refletir o que de fato aconteceu.
// Implementacoes em src/infra; as que acessam disco lancam
// FalhaArmazenamentoException.
class IAcessoRepositorio {
public:
    virtual ~IAcessoRepositorio() = default;

    // Atribui o proximo ID (nunca reaproveitado), guarda e retorna o ID.
    virtual int adicionar(const RegistroAcesso& registro) = 0;

    // Registros em ordem crescente de ID (ordem em que ocorreram).
    virtual std::vector<RegistroAcesso> listarTodos() const = 0;

    // Registros com o login informado, em ordem crescente de ID.
    virtual std::vector<RegistroAcesso> listarPorLogin(const std::string& login) const = 0;

    virtual std::size_t quantidade() const = 0;
};
