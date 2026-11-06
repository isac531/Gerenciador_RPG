#include "MenuPersonagens.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../business/exception/Excecoes.h"
#include "Console.h"

using console::lerInteiro;
using console::lerLinha;

namespace {

// Rotulo do atributo e ponteiro para o campo correspondente em Atributos.
struct CampoAtributo {
    const char* sigla;
    int Atributos::*valor;
};

const CampoAtributo kCamposAtributo[] = {
    {"FOR", &Atributos::forca},        {"DES", &Atributos::destreza},
    {"CON", &Atributos::constituicao}, {"INT", &Atributos::inteligencia},
    {"SAB", &Atributos::sabedoria},    {"CAR", &Atributos::carisma},
};

std::string comSinal(int valor) {
    return (valor >= 0 ? "+" : "") + std::to_string(valor);
}

void exibir(const Personagem& personagem, const std::string& loginDono) {
    const Ficha& ficha = personagem.getFicha();
    std::cout << "ID: " << personagem.getId() << " | Dono: " << loginDono
              << " | Nome: " << ficha.nome << " | Nivel: " << ficha.nivel
              << " | PV: " << ficha.pontosDeVida << "\n   ";
    for (const CampoAtributo& campo : kCamposAtributo) {
        const int valor = ficha.atributos.*campo.valor;
        std::cout << ' ' << campo.sigla << ' ' << valor << " (" << comSinal(calcularModificador(valor)) << ')';
    }
    std::cout << '\n';
}

// Mostra a mensagem da recusa; retorna false se a entrada terminou.
bool avisarValorInvalido() {
    if (!std::cin) {
        return false;
    }
    std::cout << "Valor invalido.\n";
    return true;
}

}  // namespace

MenuPersonagens::MenuPersonagens(FacadeSingletonController& fachada) : fachada_(fachada) {}

bool MenuPersonagens::executar() {
    while (true) {
        std::cout << "\nPersonagens\n"
                     "1. Criar personagem\n"
                     "2. Listar personagens\n"
                     "3. Editar ficha\n"
                     "4. Excluir personagem\n"
                     "0. Voltar\n";

        int opcao;
        if (!lerInteiro("Opcao: ", opcao)) {
            if (!avisarValorInvalido()) return false;
            continue;
        }

        switch (opcao) {
        case 1:
            if (!criar()) return false;
            break;
        case 2:
            listar();
            break;
        case 3:
            if (!editar()) return false;
            break;
        case 4:
            if (!excluir()) return false;
            break;
        case 0:
            return true;
        default:
            std::cout << "Opcao invalida.\n";
        }
    }
}

std::optional<Ficha> MenuPersonagens::lerFicha() {
    Ficha ficha;
    if (!lerLinha("Nome: ", ficha.nome) || !lerInteiro("Nivel (1 a 20): ", ficha.nivel) ||
        !lerInteiro("Pontos de vida: ", ficha.pontosDeVida)) {
        return std::nullopt;
    }
    for (const CampoAtributo& campo : kCamposAtributo) {
        if (!lerInteiro(std::string(campo.sigla) + " (1 a 30): ", ficha.atributos.*campo.valor)) {
            return std::nullopt;
        }
    }
    return ficha;
}

bool MenuPersonagens::criar() {
    int idDono;
    if (!lerInteiro("ID do usuario dono: ", idDono)) {
        return avisarValorInvalido();
    }
    const std::optional<Ficha> ficha = lerFicha();
    if (!ficha) {
        return avisarValorInvalido();
    }

    try {
        const int id = fachada_.criarPersonagem(Personagem(idDono, *ficha));
        std::cout << "Personagem criado com sucesso. ID: " << id << '\n';
    } catch (const IdInvalidoException& e) {
        std::cout << "Nao foi possivel criar: " << e.what() << '\n';
    } catch (const FichaInvalidaException& e) {
        std::cout << "Ficha invalida: " << e.what() << '\n';
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    }
    return true;
}

bool MenuPersonagens::editar() {
    int id;
    if (!lerInteiro("ID do personagem: ", id)) {
        return avisarValorInvalido();
    }
    const std::optional<Personagem> atual = fachada_.buscarPersonagem(id);
    if (!atual) {
        std::cout << "Personagem nao encontrado.\n";
        return true;
    }
    std::cout << "Ficha atual:\n";
    exibir(*atual, "ID " + std::to_string(atual->getIdDono()));

    const std::optional<Ficha> ficha = lerFicha();
    if (!ficha) {
        return avisarValorInvalido();
    }

    try {
        fachada_.editarPersonagem(id, *ficha);
        std::cout << "Ficha atualizada com sucesso.\n";
    } catch (const IdInvalidoException& e) {
        std::cout << "Nao foi possivel editar: " << e.what() << '\n';
    } catch (const FichaInvalidaException& e) {
        std::cout << "Ficha invalida: " << e.what() << '\n';
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    }
    return true;
}

bool MenuPersonagens::excluir() {
    int id;
    if (!lerInteiro("ID do personagem: ", id)) {
        return avisarValorInvalido();
    }

    try {
        if (fachada_.excluirPersonagem(id)) {
            std::cout << "Personagem excluido com sucesso.\n";
        } else {
            std::cout << "Personagem nao encontrado.\n";
        }
    } catch (const FalhaArmazenamentoException& e) {
        std::cout << "Falha no armazenamento: " << e.what() << '\n';
    }
    return true;
}

void MenuPersonagens::listar() const {
    const std::vector<Personagem> personagens = fachada_.listarPersonagens();
    if (personagens.empty()) {
        std::cout << "Nenhum personagem cadastrado.\n";
        return;
    }

    std::map<int, std::string> loginPorId;
    for (const Usuario& usuario : fachada_.listarUsuarios()) {
        loginPorId[usuario.getId()] = usuario.getLogin();
    }
    for (const Personagem& personagem : personagens) {
        exibir(personagem, loginPorId[personagem.getIdDono()]);
    }
}
