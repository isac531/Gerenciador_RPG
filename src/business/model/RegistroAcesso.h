#pragma once

#include <cstdint>
#include <string>

#include "Usuario.h"

// Uma tentativa de acesso (login) ao sistema, usada nos relatorios de
// estatisticas de acesso (RF13). Imutavel.
//
// - login: o login digitado (guardado mesmo que o usuario seja excluido
//   depois, para o historico continuar coerente);
// - idUsuario: ID do usuario autenticado, ou SEM_ID se o login nao existe;
// - dataHora: segundos desde 01/01/1970 (UTC);
// - sucesso: true se login e senha conferiram.
class RegistroAcesso {
public:
    RegistroAcesso() = default;

    // Registro ainda sem ID: o repositorio atribui o ID ao guardar.
    RegistroAcesso(std::string login, int idUsuario, std::int64_t dataHora, bool sucesso);

    RegistroAcesso(int id, std::string login, int idUsuario, std::int64_t dataHora,
                   bool sucesso);

    RegistroAcesso comId(int id) const;

    int getId() const;
    const std::string& getLogin() const;
    int getIdUsuario() const;
    std::int64_t getDataHora() const;
    bool foiSucesso() const;

private:
    int id_ = SEM_ID;
    std::string login_;
    int idUsuario_ = SEM_ID;
    std::int64_t dataHora_ = 0;
    bool sucesso_ = false;
};
