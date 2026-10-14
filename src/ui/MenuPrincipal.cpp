#include "MenuPrincipal.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../business/exception/Excecoes.h"
#include "../business/model/Papel.h"
#include "../infra/FabricaRepositorio.h"

bool MenuPrincipal::lerInteiro(const std::string& mensagem, int& valor) {
    std::cout << mensagem;

    std::string linha;
    if (!std::getline(std::cin, linha)) {
        return false;
    }

    std::istringstream entrada(linha);
    char caractereExtra;
    return static_cast<bool>(entrada >> valor) && !(entrada >> caractereExtra);
}

std::unique_ptr<IUsuarioRepositorio> MenuPrincipal::escolherArmazenamento() {
    while (true) {
        int opcao;
        if (!lerInteiro("Escolha o armazenamento:\n"
                        "1. Memoria (RAM)\n"
                        "2. Arquivo binario (usuarios.dat)\n"
                        "Opcao: ",
                        opcao)) {
            if (!std::cin) {
                return nullptr;
            }
            std::cout << "Opcao invalida.\n";
            continue;
        }

        if (opcao == 1) {
            std::cout << "Usando armazenamento em memoria (RAM).\n";
            return criarRepositorio(TipoArmazenamento::MEMORIA);
        }
        if (opcao == 2) {
            try {
                auto repositorio = criarRepositorio(TipoArmazenamento::ARQUIVO_BINARIO);
                std::cout << "Usando arquivo binario usuarios.dat.\n";
                return repositorio;
            } catch (const FalhaArmazenamentoException& e) {
                std::cout << "Falha no armazenamento: " << e.what() << '\n'
                          << "Usando armazenamento em memoria (RAM); "
                             "os dados nao serao salvos.\n";
                return criarRepositorio(TipoArmazenamento::MEMORIA);
            }
        }
        std::cout << "Opcao invalida.\n";
    }
}

MenuPrincipal::MenuPrincipal(ControleCriarConta& controle) : controle_(controle) {}

void MenuPrincipal::executar() {
    while (true) {
        std::cout << "\n1. Cadastrar usuario\n"
                     "2. Excluir usuario\n"
                     "3. Listar usuarios\n"
                     "0. Sair\n";

        int opcao;
        if (!lerInteiro("Opcao: ", opcao)) {
            if (!std::cin) {
                return;
            }
            std::cout << "Opcao invalida.\n";
            continue;
        }

        switch (opcao) {
        case 1:
            if (!cadastrarUsuario()) return;
            break;
        case 2:
            if (!excluirUsuario()) return;
            break;
        case 3:
            listarUsuarios();
            break;
        case 0:
            return;
        default:
            std::cout << "Opcao invalida.\n";
        }
    }
}

bool MenuPrincipal::cadastrarUsuario() {
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
        controle_.criarConta(Usuario(id, login, senha, papel));
        std::cout << "Usuario cadastrado com sucesso.\n";
    } catch (const LoginInvalidoException& e) {
        std::cout << "Login invalido: " << e.what() << '\n';
    } catch (const SenhaInvalidaException& e) {
        std::cout << "Senha invalida: " << e.what() << '\n';
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cout << "Nao foi possivel cadastrar: " << e.what() << '\n';
    }

    return true;
}

bool MenuPrincipal::excluirUsuario() {
    int id;
    if (!lerInteiro("ID do usuario: ", id)) {
        std::cout << "ID invalido.\n";
        return static_cast<bool>(std::cin);
    }

    try {
        if (controle_.excluirConta(id)) {
            std::cout << "Usuario excluido com sucesso.\n";
        } else {
            std::cout << "Usuario nao encontrado.\n";
        }
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    }

    return true;
}

void MenuPrincipal::listarUsuarios() const {
    std::vector<Usuario> usuarios;
    try {
        usuarios = controle_.listar();
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
        return;
    }

    if (usuarios.empty()) {
        std::cout << "Nenhum usuario cadastrado.\n";
        return;
    }

    // A senha nunca e exibida.
    for (const Usuario& usuario : usuarios) {
        std::cout << "ID: " << usuario.getId() << " | Login: " << usuario.getLogin()
                  << " | Papel: " << papelParaTexto(usuario.getPapel()) << '\n';
    }
}
