#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

#include "../model/Personagem.h"
#include "../model/Usuario.h"
#include "../repository/IPersonagemRepositorio.h"
#include "../repository/IUsuarioRepositorio.h"
#include "ControleCriarConta.h"
#include "ControlePersonagem.h"
#include "ControleUsuarios.h"

// Fachada unica da camada de negocio (padroes Facade e Singleton, ADR-0004).
// A interface conversa so com esta classe; ela repassa cada pedido ao
// controle do caso de uso e coordena operacoes que envolvem mais de um
// controle, como excluir um usuario junto com os personagens dele.
//
// Uso: FacadeSingletonController::getInstancia().inicializar(usuarios, personagens)
// uma vez, na raiz de composicao (main.cpp); depois, so getInstancia().
class FacadeSingletonController {
public:
    static FacadeSingletonController& getInstancia();

    FacadeSingletonController(const FacadeSingletonController&) = delete;
    FacadeSingletonController& operator=(const FacadeSingletonController&) = delete;

    // Liga os controles aos repositorios escolhidos. Chamar de novo troca
    // os repositorios (usado pelos testes).
    void inicializar(IUsuarioRepositorio& usuarios, IPersonagemRepositorio& personagens);

    // ----- Usuarios (UC01, UC03, UC04) -----
    int criarConta(const Usuario& usuario);
    std::vector<Usuario> listarUsuarios() const;
    // Exclui o usuario e os personagens dele. Retorna false se o ID nao existir.
    bool excluirUsuario(int id);

    // ----- Personagens (UC07, UC12, UC13, UC14) -----
    int criarPersonagem(const Personagem& personagem);
    void editarPersonagem(int id, const Ficha& ficha);
    bool excluirPersonagem(int id);
    std::optional<Personagem> buscarPersonagem(int id) const;
    std::vector<Personagem> listarPersonagens() const;

    // ----- Quantidade de entidades cadastradas (RF12) -----
    std::size_t obterQuantidadeUsuarios() const;
    std::size_t obterQuantidadePersonagens() const;
    // Usuarios + personagens.
    std::size_t obterQuantidadeEntidades() const;

private:
    FacadeSingletonController() = default;

    // Lancam std::logic_error se inicializar() ainda nao foi chamado.
    ControleCriarConta& controleCriarConta() const;
    ControleUsuarios& controleUsuarios() const;
    ControlePersonagem& controlePersonagem() const;

    std::unique_ptr<ControleCriarConta> controleCriarConta_;
    std::unique_ptr<ControleUsuarios> controleUsuarios_;
    std::unique_ptr<ControlePersonagem> controlePersonagem_;
};
