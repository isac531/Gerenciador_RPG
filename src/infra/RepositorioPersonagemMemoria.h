#pragma once

#include <map>

#include "../business/repository/IPersonagemRepositorio.h"

// Guarda os personagens apenas na RAM; os dados se perdem ao sair.
class RepositorioPersonagemMemoria : public IPersonagemRepositorio {
public:
    int adicionar(const Personagem& personagem) override;
    bool atualizar(const Personagem& personagem) override;
    bool remover(int id) override;
    std::optional<Personagem> buscar(int id) const override;
    std::vector<Personagem> listarTodos() const override;
    std::size_t quantidade() const override;

protected:
    // Estado completo, usado pela versao em arquivo para carregar os dados
    // e para desfazer uma alteracao quando a gravacao falha.
    struct Estado {
        std::map<int, Personagem> personagens;
        int proximoId = 1;
    };

    Estado estado_;
};
