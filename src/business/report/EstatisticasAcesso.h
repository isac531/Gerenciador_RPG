#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../model/Papel.h"

// Estatisticas de acesso de um usuario cadastrado.
struct EstatisticaUsuario {
    int idUsuario = 0;
    std::string login;
    Papel papel = Papel::JOGADOR;
    std::size_t acessos = 0;  // logins aceitos
    std::size_t recusas = 0;  // senha errada para este login
    std::optional<std::int64_t> ultimoAcesso;  // ultimo login aceito
};

// Dados de um relatorio de acessos (RF14), calculados a partir dos
// registros de acesso e dos usuarios cadastrados. Independe do formato.
struct EstatisticasAcesso {
    std::int64_t geradoEm = 0;
    std::size_t totalTentativas = 0;
    std::size_t acessosAceitos = 0;
    std::size_t tentativasRecusadas = 0;
    // Tentativas com um login que nao existe (entram nas recusadas).
    std::size_t tentativasLoginInexistente = 0;
    std::size_t usuariosCadastrados = 0;
    std::size_t usuariosQueAcessaram = 0;
    std::optional<std::int64_t> primeiraTentativa;
    std::optional<std::int64_t> ultimaTentativa;
    // Todos os usuarios cadastrados, em ordem crescente de ID.
    std::vector<EstatisticaUsuario> porUsuario;

    // Percentual de tentativas aceitas (0 se nao houve tentativas).
    double taxaDeSucesso() const;

    // Usuario com mais acessos aceitos (o de menor ID em caso de empate);
    // vazio se ninguem acessou.
    std::optional<EstatisticaUsuario> usuarioMaisAtivo() const;
};
