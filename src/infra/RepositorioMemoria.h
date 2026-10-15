#pragma once

#include <map>

#include "../business/repository/IUsuarioRepositorio.h"

// Armazena os usuarios apenas na RAM; os dados se perdem ao sair.
class RepositorioMemoria : public IUsuarioRepositorio {
public:
    bool adicionar(const Usuario& usuario) override;
    bool remover(int id) override;
    bool existe(int id) const override;
    std::optional<Usuario> buscar(int id) const override;
    std::vector<Usuario> listarTodos() const override;

private:
    std::map<int, Usuario> usuarios_;
};
