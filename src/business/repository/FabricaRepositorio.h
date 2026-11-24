#pragma once

#include <memory>

#include "IAcessoRepositorio.h"
#include "IPersonagemRepositorio.h"
#include "IUsuarioRepositorio.h"
#include "TipoArmazenamento.h"

// Conjunto de repositorios de um mesmo mecanismo de armazenamento.
struct Repositorios {
    std::unique_ptr<IUsuarioRepositorio> usuarios;
    std::unique_ptr<IPersonagemRepositorio> personagens;
    std::unique_ptr<IAcessoRepositorio> acessos;
};

// Abstract Factory (padrao de criacao), no papel de AbstractFactory.
//
// Esta classe abstrata fica na camada business: a business sabe QUE precisa
// de repositorios e O QUE eles fazem (as interfaces I...Repositorio), mas nao
// sabe COMO nem ONDE os dados sao guardados. Cada metodo criar...() declara a
// criacao de um produto da familia (usuarios, personagens, acessos); a
// subclasse (em src/infra) cria a familia completa de um mesmo mecanismo,
// sem misturar repositorios de mecanismos diferentes.
//
// Para um novo mecanismo (ex.: banco de dados), cria-se uma nova subclasse;
// nenhum controle, fachada ou teste de regra de negocio precisa mudar.
// Para uma nova entidade, acrescenta-se uma operacao de criacao aqui e uma
// implementacao em cada subclasse.
//
// Os repositorios criados podem lancar FalhaArmazenamentoException
// (ex.: arquivo corrompido).
class FabricaRepositorio {
public:
    virtual ~FabricaRepositorio() = default;

    // ----- Criacao dos produtos da familia (implementada pelas subclasses) -----
    virtual std::unique_ptr<IUsuarioRepositorio> criarRepositorioUsuarios() const = 0;
    virtual std::unique_ptr<IPersonagemRepositorio> criarRepositorioPersonagens() const = 0;
    virtual std::unique_ptr<IAcessoRepositorio> criarRepositorioAcessos() const = 0;

    // Mecanismo de armazenamento que esta fabrica produz.
    virtual TipoArmazenamento tipo() const = 0;

    // Cria o conjunto completo de repositorios usando as operacoes acima, sem
    // conhecer as classes concretas.
    Repositorios criarRepositorios() const {
        Repositorios repositorios;
        repositorios.usuarios = criarRepositorioUsuarios();
        repositorios.personagens = criarRepositorioPersonagens();
        repositorios.acessos = criarRepositorioAcessos();
        return repositorios;
    }
};
