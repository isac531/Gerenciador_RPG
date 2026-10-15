#pragma once

#include <optional>
#include <string>

#include "../business/control/ControleCriarConta.h"
#include "../business/control/ControleUsuarios.h"
#include "../business/repository/TipoArmazenamento.h"

// Interface de console: le a entrada do usuario e chama o controle.
class MenuPrincipal {
public:
    // Pergunta qual armazenamento usar.
    // Retorna std::nullopt apenas se a entrada padrao for encerrada.
    static std::optional<TipoArmazenamento> escolherArmazenamento();

    MenuPrincipal(ControleCriarConta& controleCriarConta, ControleUsuarios& controleUsuarios);

    // Laco principal do menu; termina com a opcao 0 ou fim da entrada.
    void executar();

private:
    static bool lerInteiro(const std::string& mensagem, int& valor);

    // Retornam false apenas quando a entrada padrao foi encerrada.
    bool cadastrarUsuario();
    bool excluirUsuario();
    void listarUsuarios() const;

    ControleCriarConta& controleCriarConta_;
    ControleUsuarios& controleUsuarios_;
};
