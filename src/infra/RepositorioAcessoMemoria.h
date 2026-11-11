#pragma once

#include <map>

#include "../business/repository/IAcessoRepositorio.h"

// Guarda os registros de acesso apenas na RAM; os dados se perdem ao sair.
class RepositorioAcessoMemoria : public IAcessoRepositorio {
public:
    int adicionar(const RegistroAcesso& registro) override;
    std::vector<RegistroAcesso> listarTodos() const override;
    std::vector<RegistroAcesso> listarPorLogin(const std::string& login) const override;
    std::size_t quantidade() const override;

protected:
    // Estado completo, usado pela versao em arquivo para carregar os dados
    // e para desfazer uma alteracao quando a gravacao falha.
    struct Estado {
        std::map<int, RegistroAcesso> registros;
        int proximoId = 1;
    };

    Estado estado_;
};
