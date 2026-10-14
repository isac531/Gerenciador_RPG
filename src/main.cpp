#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>

#include "business/control/ControleCriarConta.h"
#include "business/model/Usuario.h"
#include "infra/RepositorioMemoria.h"

bool lerInteiro(const std::string& mensagem, int& valor) {
    std::cout << mensagem;

    std::string linha;
    if (!std::getline(std::cin, linha)) {
        return false;
    }

    std::istringstream entrada(linha);
    char caractereExtra;
    return static_cast<bool>(entrada >> valor) && !(entrada >> caractereExtra);
}

// Retorna false apenas quando a entrada padrao foi encerrada.
bool cadastrarUsuario(ControleCriarConta& controle) {
    int id;
    if (!lerInteiro("ID: ", id)) {
        std::cout << "ID invalido.\n";
        return static_cast<bool>(std::cin);
    }

    std::string login;
    std::cout << "Login: ";
    if (!std::getline(std::cin, login)) {
        return false;
    }

    std::string senha;
    std::cout << "Senha: ";
    if (!std::getline(std::cin, senha)) {
        return false;
    }

    int codigoPapel;
    Papel papel;
    if (!lerInteiro("Papel (1. Mestre  2. Jogador): ", codigoPapel) ||
        !papelDeCodigo(codigoPapel, papel)) {
        std::cout << "Papel invalido.\n";
        return static_cast<bool>(std::cin);
    }

    try {
        controle.criarConta(Usuario(id, login, senha, papel));
        std::cout << "Usuario cadastrado com sucesso.\n";
    } catch (const LoginInvalidoException& e) {
        std::cout << "Login invalido: " << e.what() << '\n';
    } catch (const SenhaInvalidaException& e) {
        std::cout << "Senha invalida: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cout << "Nao foi possivel cadastrar: " << e.what() << '\n';
    }

    return true;
}

bool excluirUsuario(ControleCriarConta& controle) {
    int id;
    if (!lerInteiro("ID do usuario: ", id)) {
        std::cout << "ID invalido.\n";
        return static_cast<bool>(std::cin);
    }

    if (controle.excluirConta(id)) {
        std::cout << "Usuario excluido com sucesso.\n";
    } else {
        std::cout << "Usuario nao encontrado.\n";
    }

    return true;
}

void listarUsuarios(const ControleCriarConta& controle) {
    const std::vector<Usuario> usuarios = controle.listar();
    if (usuarios.empty()) {
        std::cout << "Nenhum usuario cadastrado.\n";
        return;
    }

    // A senha nunca e exibida.
    std::cout << "ID | Login | Papel\n";
    for (const Usuario& usuario : usuarios) {
        std::cout << usuario.getId() << " | " << usuario.getLogin() << " | "
                  << papelParaTexto(usuario.getPapel()) << '\n';
    }
}

int main() {
    ControleCriarConta controle(std::make_unique<RepositorioMemoria>());

    while (true) {
        std::cout << "\n1. Cadastrar usuario\n"
                     "2. Excluir usuario\n"
                     "3. Listar usuarios\n"
                     "0. Sair\n";

        int opcao;
        if (!lerInteiro("Opcao: ", opcao)) {
            if (!std::cin) {
                break;
            }
            std::cout << "Opcao invalida.\n";
            continue;
        }

        switch (opcao) {
        case 1:
            if (!cadastrarUsuario(controle)) return 0;
            break;
        case 2:
            if (!excluirUsuario(controle)) return 0;
            break;
        case 3:
            listarUsuarios(controle);
            break;
        case 0:
            return 0;
        default:
            std::cout << "Opcao invalida.\n";
        }
    }

    return 0;
}
