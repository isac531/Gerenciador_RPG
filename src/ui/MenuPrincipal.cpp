#include "MenuPrincipal.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../business/exception/Excecoes.h"
#include "../business/model/Papel.h"
#include "Console.h"
#include "MenuPersonagens.h"

using console::lerInteiro;
using console::lerLinha;

std::optional<TipoArmazenamento> MenuPrincipal::escolherArmazenamento() {
    while (true) {
        int opcao;
        if (!lerInteiro("Escolha o armazenamento:\n"
                        "1. Memoria (RAM)\n"
                        "2. Arquivo binario (usuarios.dat, personagens.dat e acessos.dat)\n"
                        "Opcao: ",
                        opcao)) {
            if (!std::cin) {
                return std::nullopt;
            }
            std::cout << "Opcao invalida.\n";
            continue;
        }

        if (opcao == 1) {
            return TipoArmazenamento::MEMORIA;
        }
        if (opcao == 2) {
            return TipoArmazenamento::ARQUIVO_BINARIO;
        }
        std::cout << "Opcao invalida.\n";
    }
}

MenuPrincipal::MenuPrincipal(FacadeSingletonController& fachada) : fachada_(fachada) {}

void MenuPrincipal::executar() {
    while (true) {
        std::cout << "\n1. Cadastrar usuario\n"
                     "2. Excluir usuario\n"
                     "3. Listar usuarios\n"
                     "4. Personagens\n"
                     "5. Quantidade de entidades cadastradas\n"
                     "6. Entrar (login)\n"
                     "7. Relatorio de acessos (HTML ou PDF)\n"
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
        case 4:
            if (!MenuPersonagens(fachada_).executar()) return;
            break;
        case 5:
            exibirQuantidadeEntidades();
            break;
        case 6:
            if (!fazerLogin()) return;
            break;
        case 7:
            if (!gerarRelatorioAcessos()) return;
            break;
        case 0:
            return;
        default:
            std::cout << "Opcao invalida.\n";
        }
    }
}

bool MenuPrincipal::cadastrarUsuario() {
    std::string login;
    std::string senha;
    if (!lerLinha("Login: ", login) || !lerLinha("Senha: ", senha)) {
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
        const int id = fachada_.criarConta(Usuario(login, senha, papel));
        std::cout << "Usuario cadastrado com sucesso. ID: " << id << '\n';
    } catch (const LoginInvalidoException& e) {
        std::cout << "Login invalido: " << e.what() << '\n';
    } catch (const SenhaInvalidaException& e) {
        std::cout << "Senha invalida: " << e.what() << '\n';
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
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
        if (fachada_.excluirUsuario(id)) {
            std::cout << "Usuario e seus personagens excluidos com sucesso.\n";
        } else {
            std::cout << "Usuario nao encontrado.\n";
        }
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    }

    return true;
}

bool MenuPrincipal::fazerLogin() {
    std::string login;
    std::string senha;
    if (!lerLinha("Login: ", login) || !lerLinha("Senha: ", senha)) {
        return false;
    }

    try {
        const Usuario usuario = fachada_.fazerLogin(login, senha);
        std::cout << "Bem-vindo, " << usuario.getLogin() << " ("
                  << papelParaTexto(usuario.getPapel()) << ").\n";
    } catch (const CredenciaisInvalidasException& e) {
        std::cout << e.what() << '\n';
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    }

    return true;
}

bool MenuPrincipal::gerarRelatorioAcessos() {
    int codigo;
    if (!lerInteiro("Formato (1. HTML  2. PDF): ", codigo)) {
        std::cout << "Formato invalido.\n";
        return static_cast<bool>(std::cin);
    }
    const auto formato = formatoDeCodigo(codigo);
    if (!formato) {
        std::cout << "Formato invalido.\n";
        return true;
    }

    const RelatorioGerado relatorio = fachada_.gerarRelatorioAcessos(*formato);
    const std::string caminho = "relatorio-acessos." + relatorio.extensao;
    std::ofstream arquivo(caminho, std::ios::binary | std::ios::trunc);
    arquivo << relatorio.conteudo;
    arquivo.close();
    if (!arquivo) {
        std::cout << "Nao foi possivel gravar o arquivo " << caminho << ".\n";
    } else {
        std::cout << "Relatorio gravado em " << caminho << ".\n";
    }
    return true;
}

void MenuPrincipal::listarUsuarios() const {
    const std::vector<Usuario> usuarios = fachada_.listarUsuarios();

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

void MenuPrincipal::exibirQuantidadeEntidades() const {
    std::cout << "Entidades cadastradas: " << fachada_.obterQuantidadeEntidades()
              << " (usuarios: " << fachada_.obterQuantidadeUsuarios()
              << ", personagens: " << fachada_.obterQuantidadePersonagens() << ")\n";
}
