#include <string>

#include "../src/business/control/ControleLogin.h"
#include "../src/business/control/FacadeSingletonController.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "TestRunner.h"

namespace {

constexpr std::int64_t kInstante = 1794398400;  // 11/11/2026 12:00:00 UTC

struct CenarioLogin {
    RepositorioMemoria usuarios;
    RepositorioAcessoMemoria acessos;
    ControleLogin controle{usuarios, acessos, [] { return kInstante; }};

    CenarioLogin() { usuarios.adicionar(Usuario("isac", "Dado#2026", Papel::MESTRE)); }
};

}  // namespace

// ---------- UC02 Fazer login (RF07) e registro de acessos (RF13) ----------

TESTE(deve_autenticar_com_login_e_senha_corretos) {
    CenarioLogin cenario;
    const Usuario usuario = cenario.controle.autenticar("isac", "Dado#2026");
    VERIFICAR(usuario.getId() == 1);
    VERIFICAR(usuario.getPapel() == Papel::MESTRE);
}

TESTE(deve_recusar_senha_incorreta_sem_indicar_o_campo) {
    CenarioLogin cenario;
    try {
        cenario.controle.autenticar("isac", "Errada#2026");
        FALHAR("esperado lancar CredenciaisInvalidasException");
    } catch (const CredenciaisInvalidasException& e) {
        VERIFICAR(std::string(e.what()) == "Login ou senha invalidos.");
    }
}

TESTE(deve_recusar_login_inexistente_com_a_mesma_mensagem) {
    CenarioLogin cenario;
    VERIFICAR_LANCA(cenario.controle.autenticar("ninguem", "Dado#2026"),
                    CredenciaisInvalidasException);
}

TESTE(login_aceito_deve_ser_registrado_como_sucesso) {
    CenarioLogin cenario;
    cenario.controle.autenticar("isac", "Dado#2026");
    const auto registros = cenario.acessos.listarTodos();
    VERIFICAR(registros.size() == 1);
    VERIFICAR(registros[0].getLogin() == "isac");
    VERIFICAR(registros[0].getIdUsuario() == 1);
    VERIFICAR(registros[0].getDataHora() == kInstante);
    VERIFICAR(registros[0].foiSucesso());
}

TESTE(login_recusado_deve_ser_registrado_como_falha) {
    CenarioLogin cenario;
    VERIFICAR_LANCA(cenario.controle.autenticar("isac", "errada"), CredenciaisInvalidasException);
    VERIFICAR_LANCA(cenario.controle.autenticar("bia", "Dado#2026"), CredenciaisInvalidasException);
    const auto registros = cenario.acessos.listarTodos();
    VERIFICAR(registros.size() == 2);
    VERIFICAR(!registros[0].foiSucesso());
    VERIFICAR(registros[0].getIdUsuario() == 1);
    VERIFICAR(!registros[1].foiSucesso());
    VERIFICAR(registros[1].getIdUsuario() == SEM_ID);
}

TESTE(fachada_deve_fazer_login_e_registrar_o_acesso) {
    RepositorioMemoria usuarios;
    RepositorioPersonagemMemoria personagens;
    RepositorioAcessoMemoria acessos;
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();
    fachada.inicializar(usuarios, personagens, acessos, [] { return kInstante; });
    fachada.criarConta(Usuario("bia", "Senha@123", Papel::JOGADOR));

    VERIFICAR(fachada.fazerLogin("bia", "Senha@123").getLogin() == "bia");
    VERIFICAR(acessos.quantidade() == 1);
}
