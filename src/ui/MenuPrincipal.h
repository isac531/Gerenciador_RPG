#pragma once

#include <optional>

#include "../business/control/FacadeSingletonController.h"
#include "../business/repository/TipoArmazenamento.h"

// Interface de console: le a entrada do usuario e chama a fachada.
class MenuPrincipal {
public:
    // Pergunta qual armazenamento usar.
    // Retorna std::nullopt apenas se a entrada padrao for encerrada.
    static std::optional<TipoArmazenamento> escolherArmazenamento();

    explicit MenuPrincipal(FacadeSingletonController& fachada);

    // Laco principal do menu; termina com a opcao 0 ou fim da entrada.
    void executar();

private:
    // Retornam false apenas quando a entrada padrao foi encerrada.
    bool cadastrarUsuario();
    bool excluirUsuario();
    void listarUsuarios() const;
    void exibirQuantidadeEntidades() const;

    FacadeSingletonController& fachada_;
};
