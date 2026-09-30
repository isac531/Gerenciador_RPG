#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

struct Usuario {
    int id;
    std::string nome;
    std::string senha;
};

class GerenciadorUsuarios {
public:
    bool cadastrar(Usuario usuario) {
        if (usuario.id <= 0 || !temConteudo(usuario.nome) ||
            !temConteudo(usuario.senha)) {
            return false;
        }

        return usuarios_.emplace(usuario.id, std::move(usuario)).second;
    }

    bool excluir(int id) {
        return usuarios_.erase(id) > 0;
    }

    const std::map<int, Usuario>& listar() const {
        return usuarios_;
    }

private:
    static bool temConteudo(const std::string& texto) {
        for (unsigned char caractere : texto) {
            if (!std::isspace(caractere)) {
                return true;
            }
        }
        return false;
    }

    std::map<int, Usuario> usuarios_;
};

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

bool cadastrarUsuario(GerenciadorUsuarios& gerenciador) {
    int id;
    if (!lerInteiro("ID: ", id)) {
        std::cout << "ID invalido.\n";
        return static_cast<bool>(std::cin);
    }

    Usuario usuario{id, {}, {}};
    std::cout << "Nome: ";
    if (!std::getline(std::cin, usuario.nome)) {
        return false;
    }

    std::cout << "Senha: ";
    if (!std::getline(std::cin, usuario.senha)) {
        return false;
    }

    if (gerenciador.cadastrar(std::move(usuario))) {
        std::cout << "Usuario cadastrado com sucesso.\n";
    } else {
        std::cout << "Nao foi possivel cadastrar. Verifique os campos e se o ID ja existe.\n";
    }

    return true;
}

bool excluirUsuario(GerenciadorUsuarios& gerenciador) {
    int id;
    if (!lerInteiro("ID do usuario: ", id)) {
        std::cout << "ID invalido.\n";
        return static_cast<bool>(std::cin);
    }

    if (gerenciador.excluir(id)) {
        std::cout << "Usuario excluido com sucesso.\n";
    } else {
        std::cout << "Usuario nao encontrado.\n";
    }

    return true;
}

void listarUsuarios(const GerenciadorUsuarios& gerenciador) {
    if (gerenciador.listar().empty()) {
        std::cout << "Nenhum usuario cadastrado.\n";
        return;
    }

    for (const auto& entrada : gerenciador.listar()) {
        std::cout << "ID: " << entrada.first << " | Nome: "
                  << entrada.second.nome << '\n';
    }
}

int main() {
    GerenciadorUsuarios gerenciador;

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
            if (!cadastrarUsuario(gerenciador)) {
                return 0;
            }
            break;
        case 2:
            if (!excluirUsuario(gerenciador)) {
                return 0;
            }
            break;
        case 3:
            listarUsuarios(gerenciador);
            break;
        case 0:
            return 0;
        default:
            std::cout << "Opcao invalida.\n";
        }
    }

    return 0;
}