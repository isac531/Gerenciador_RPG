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

// Factory Method (padrao de criacao), no papel de Creator.
//
// Esta classe abstrata fica na camada business: a business sabe QUE precisa
// de repositorios e O QUE eles fazem (as interfaces I...Repositorio), mas nao
// sabe COMO nem ONDE os dados sao guardados. Cada metodo criar...() e um
// factory method: so declara a criacao; a subclasse (em src/infra) decide
// qual classe concreta instanciar.
//
// Para um novo mecanismo (ex.: banco de dados), cria-se uma nova subclasse;
// nenhum controle, fachada ou teste de regra de negocio precisa mudar.
// Para uma nova entidade, acrescenta-se um factory method aqui e uma
// implementacao em cada subclasse.
//
// Os repositorios criados podem lancar FalhaArmazenamentoException
// (ex.: arquivo corrompido).
class FabricaRepositorio {
public:
    virtual ~FabricaRepositorio() = default;

    // ----- Factory methods (implementados pelas subclasses) -----
    virtual std::unique_ptr<IUsuarioRepositorio> criarRepositorioUsuarios() const = 0;
    virtual std::unique_ptr<IPersonagemRepositorio> criarRepositorioPersonagens() const = 0;
    virtual std::unique_ptr<IAcessoRepositorio> criarRepositorioAcessos() const = 0;

    // Mecanismo de armazenamento que esta fabrica produz.
    virtual TipoArmazenamento tipo() const = 0;

    // Operacao do Creator que usa os factory methods sem conhecer as classes
    // concretas: cria o conjunto completo de repositorios.
    Repositorios criarRepositorios() const {
        Repositorios repositorios;
        repositorios.usuarios = criarRepositorioUsuarios();
        repositorios.personagens = criarRepositorioPersonagens();
        repositorios.acessos = criarRepositorioAcessos();
        return repositorios;
    }
};
