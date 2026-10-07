#include <iostream>
#include <sstream>
#include <string>

#include "control/ControleCriarConta.h"
#include "entity/Usuario.h"

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

    std::string nome;
    std::cout << "Nome: ";
    if (!std::getline(std::cin, nome)) {
        return false;
    }

    std::string senha;
    std::cout << "Senha: ";
    if (!std::getline(std::cin, senha)) {
        return false;
    }

    try {
        controle.criarConta(Usuario(id, nome, senha));
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
    if (controle.listar().empty()) {
        std::cout << "Nenhum usuario cadastrado.\n";
        return;
    }

    for (const auto& entrada : controle.listar()) {
        std::cout << "ID: " << entrada.first
                  << " | Nome: " << entrada.second.getNome() << '\n';
    }
}

int main() {
    ControleCriarConta controle;

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
