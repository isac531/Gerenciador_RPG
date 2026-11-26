#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

#include "../memento/MementoPersonagem.h"
#include "../model/Personagem.h"
#include "../repository/IPersonagemRepositorio.h"
#include "../repository/IUsuarioRepositorio.h"
#include "../validation/IRegraFicha.h"

// Controle do CRUD de personagens (UC07, UC12, UC13, UC14).
class ControlePersonagem {
public:
    ControlePersonagem(IPersonagemRepositorio& personagens, const IUsuarioRepositorio& usuarios);

    // Valida o dono e a ficha e cadastra; retorna o ID gerado.
    // Lanca IdInvalidoException (dono inexistente), FichaInvalidaException
    // (e subclasses) ou FalhaArmazenamentoException.
    int criar(const Personagem& personagem);

    // Substitui a ficha do personagem `id`; o dono nao muda. Retorna o
    // memento do estado anterior, para desfazer a edicao (RF15).
    // Lanca IdInvalidoException (personagem inexistente), FichaInvalidaException
    // ou FalhaArmazenamentoException.
    MementoPersonagem editarFicha(int id, const Ficha& ficha);

    // Volta o personagem do memento a ficha guardada e grava (RF15). A
    // ficha nao e validada de novo: ja tinha sido aceita antes. Retorna o
    // personagem restaurado, ou std::nullopt se ele nao existe mais.
    // Lanca FalhaArmazenamentoException.
    std::optional<Personagem> restaurar(const MementoPersonagem& memento);

    // Retorna false se o ID nao existir.
    bool excluir(int id);

    // Exclui todos os personagens do usuario; retorna quantos foram excluidos.
    std::size_t excluirDoDono(int idDono);

    std::optional<Personagem> buscar(int id) const;
    std::vector<Personagem> listar() const;
    std::size_t quantidade() const;

private:
    void validarFicha(const Ficha& ficha) const;

    IPersonagemRepositorio& personagens_;
    const IUsuarioRepositorio& usuarios_;
    std::vector<std::unique_ptr<IRegraFicha>> regrasFicha_;
};
