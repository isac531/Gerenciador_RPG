#pragma once

#include <string>

#include "../business/repository/FabricaRepositorio.h"

// Creator concreto: produz repositorios que gravam em arquivos binarios.
// Cada entidade tem o seu arquivo; os caminhos sao definidos aqui, e nao
// espalhados pelo restante do sistema.
class FabricaArquivoBinario : public FabricaRepositorio {
public:
    explicit FabricaArquivoBinario(std::string caminhoUsuarios = "usuarios.dat",
                                   std::string caminhoPersonagens = "personagens.dat",
                                   std::string caminhoAcessos = "acessos.dat");

    std::unique_ptr<IUsuarioRepositorio> criarRepositorioUsuarios() const override;
    std::unique_ptr<IPersonagemRepositorio> criarRepositorioPersonagens() const override;
    std::unique_ptr<IAcessoRepositorio> criarRepositorioAcessos() const override;
    TipoArmazenamento tipo() const override;

private:
    std::string caminhoUsuarios_;
    std::string caminhoPersonagens_;
    std::string caminhoAcessos_;
};
