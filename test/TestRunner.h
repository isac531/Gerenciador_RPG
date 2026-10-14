#pragma once

// Executor de testes minimo, sem dependencias externas.
// Uso:
//   TESTE(nome_do_teste) { VERIFICAR(condicao); VERIFICAR_LANCA(expr, Tipo); }
//   int main() { return executarTestes(); }

#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct FalhaTeste {
    std::string mensagem;
};

struct CasoTeste {
    std::string nome;
    std::function<void()> corpo;
};

inline std::vector<CasoTeste>& casosTeste() {
    static std::vector<CasoTeste> casos;
    return casos;
}

struct RegistroTeste {
    RegistroTeste(const char* nome, std::function<void()> corpo) {
        casosTeste().push_back({nome, std::move(corpo)});
    }
};

#define TESTE(nome)                                         \
    static void nome();                                     \
    static RegistroTeste registro_##nome(#nome, nome);      \
    static void nome()

#define FALHAR(texto)                                                          \
    throw FalhaTeste{std::string(__FILE__) + ":" + std::to_string(__LINE__) + \
                     ": " + (texto)}

#define VERIFICAR(condicao)                                \
    do {                                                   \
        if (!(condicao)) FALHAR("esperado: " #condicao);   \
    } while (0)

#define VERIFICAR_LANCA(expressao, TipoExcecao)                                 \
    do {                                                                        \
        bool lancou_ = false;                                                   \
        try {                                                                   \
            expressao;                                                          \
        } catch (const TipoExcecao&) {                                          \
            lancou_ = true;                                                     \
        } catch (const std::exception& e) {                                     \
            FALHAR(std::string("lancou outra excecao: ") + e.what());           \
        }                                                                       \
        if (!lancou_) FALHAR("esperado lancar " #TipoExcecao ": " #expressao);  \
    } while (0)

#define VERIFICAR_NAO_LANCA(expressao)                                          \
    do {                                                                        \
        try {                                                                   \
            expressao;                                                          \
        } catch (const std::exception& e) {                                     \
            FALHAR(std::string("excecao inesperada: ") + e.what());             \
        }                                                                       \
    } while (0)

inline int executarTestes() {
    int passou = 0;
    int falhou = 0;
    for (const CasoTeste& caso : casosTeste()) {
        try {
            caso.corpo();
            ++passou;
            std::cout << "[ OK ] " << caso.nome << '\n';
        } catch (const FalhaTeste& falha) {
            ++falhou;
            std::cout << "[FALHOU] " << caso.nome << "\n         " << falha.mensagem << '\n';
        } catch (const std::exception& e) {
            ++falhou;
            std::cout << "[FALHOU] " << caso.nome << "\n         excecao: " << e.what() << '\n';
        }
    }
    std::cout << '\n' << passou << " passaram, " << falhou << " falharam, "
              << (passou + falhou) << " no total.\n";
    return falhou == 0 ? 0 : 1;
}
