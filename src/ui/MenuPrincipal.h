#pragma once

#include <memory>
#include <string>

#include "../business/control/ControleCriarConta.h"
#include "../business/repository/IUsuarioRepositorio.h"

// Interface de console: le a entrada do usuario e chama o controle.
class MenuPrincipal {
public:
    // Pergunta qual armazenamento usar e o constroi. Se o arquivo binario
    // falhar, informa o erro e usa a memoria (RAM).
    // Retorna nullptr apenas se a entrada padrao for encerrada.
    static std::unique_ptr<IUsuarioRepositorio> escolherArmazenamento();

    explicit MenuPrincipal(ControleCriarConta& controle);

    // Laco principal do menu; termina com a opcao 0 ou fim da entrada.
    void executar();

private:
    static bool lerInteiro(const std::string& mensagem, int& valor);

    // Retornam false apenas quando a entrada padrao foi encerrada.
    bool cadastrarUsuario();
    bool excluirUsuario();
    void listarUsuarios() const;

    ControleCriarConta& controle_;
};
