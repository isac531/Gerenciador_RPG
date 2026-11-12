#include <string>

#include "../src/business/control/FacadeSingletonController.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "TestRunner.h"

namespace {

const std::string kSenhaValida = "Senha@123";

// Repositorios novos em memoria ligados a fachada, para isolar cada teste.
struct CenarioFachada {
    RepositorioMemoria usuarios;
    RepositorioPersonagemMemoria personagens;
    RepositorioAcessoMemoria acessos;
    FacadeSingletonController& fachada = FacadeSingletonController::getInstancia();

    CenarioFachada() { fachada.inicializar(usuarios, personagens, acessos); }

    int criarUsuario(const std::string& login) {
        return fachada.criarConta(Usuario(login, kSenhaValida, Papel::JOGADOR));
    }
};

Ficha fichaValida(const std::string& nome) {
    Ficha ficha;
    ficha.nome = nome;
    ficha.nivel = 1;
    ficha.pontosDeVida = 10;
    return ficha;
}

}  // namespace

TESTE(fachada_deve_ser_instancia_unica) {
    VERIFICAR(&FacadeSingletonController::getInstancia() == &FacadeSingletonController::getInstancia());
}

TESTE(fachada_deve_contar_entidades_cadastradas) {
    CenarioFachada cenario;
    VERIFICAR(cenario.fachada.obterQuantidadeEntidades() == 0);

    const int isac = cenario.criarUsuario("isac");
    cenario.criarUsuario("bia");
    cenario.fachada.criarPersonagem(Personagem(isac, fichaValida("Thorin")));

    VERIFICAR(cenario.fachada.obterQuantidadeUsuarios() == 2);
    VERIFICAR(cenario.fachada.obterQuantidadePersonagens() == 1);
    VERIFICAR(cenario.fachada.obterQuantidadeEntidades() == 3);
}

TESTE(deve_criar_personagem_com_id_automatico) {
    CenarioFachada cenario;
    const int dono = cenario.criarUsuario("isac");
    VERIFICAR(cenario.fachada.criarPersonagem(Personagem(dono, fichaValida("Thorin"))) == 1);
    VERIFICAR(cenario.fachada.criarPersonagem(Personagem(dono, fichaValida("Lia"))) == 2);
    VERIFICAR(cenario.fachada.listarPersonagens().size() == 2);
}

TESTE(deve_recusar_personagem_de_usuario_inexistente) {
    CenarioFachada cenario;
    VERIFICAR_LANCA(cenario.fachada.criarPersonagem(Personagem(42, fichaValida("Thorin"))),
                    IdInvalidoException);
    VERIFICAR(cenario.fachada.obterQuantidadePersonagens() == 0);
}

TESTE(deve_recusar_personagem_com_ficha_invalida) {
    CenarioFachada cenario;
    const int dono = cenario.criarUsuario("isac");
    Ficha ficha = fichaValida("Thorin");
    ficha.nivel = 21;
    VERIFICAR_LANCA(cenario.fachada.criarPersonagem(Personagem(dono, ficha)), NivelInvalidoException);
    VERIFICAR(cenario.fachada.obterQuantidadePersonagens() == 0);
}

TESTE(deve_editar_ficha_mantendo_o_dono) {
    CenarioFachada cenario;
    const int dono = cenario.criarUsuario("isac");
    const int id = cenario.fachada.criarPersonagem(Personagem(dono, fichaValida("Thorin")));

    Ficha nova = fichaValida("Thorin");
    nova.nivel = 2;
    nova.pontosDeVida = 19;
    cenario.fachada.editarPersonagem(id, nova);

    const auto editado = cenario.fachada.buscarPersonagem(id);
    VERIFICAR(editado && editado->getIdDono() == dono);
    VERIFICAR(editado->getFicha().nivel == 2 && editado->getFicha().pontosDeVida == 19);
}

TESTE(deve_recusar_edicao_com_ficha_invalida_sem_alterar) {
    CenarioFachada cenario;
    const int id = cenario.fachada.criarPersonagem(
        Personagem(cenario.criarUsuario("isac"), fichaValida("Thorin")));
    VERIFICAR_LANCA(cenario.fachada.editarPersonagem(id, fichaValida("")), NomePersonagemInvalidoException);
    VERIFICAR(cenario.fachada.buscarPersonagem(id)->getFicha().nome == "Thorin");
}

TESTE(editar_personagem_inexistente_deve_lancar_id_invalido) {
    CenarioFachada cenario;
    VERIFICAR_LANCA(cenario.fachada.editarPersonagem(9, fichaValida("Thorin")), IdInvalidoException);
}

TESTE(deve_excluir_personagem) {
    CenarioFachada cenario;
    const int id = cenario.fachada.criarPersonagem(
        Personagem(cenario.criarUsuario("isac"), fichaValida("Thorin")));
    VERIFICAR(cenario.fachada.excluirPersonagem(id));
    VERIFICAR(!cenario.fachada.excluirPersonagem(id));
    VERIFICAR(cenario.fachada.listarPersonagens().empty());
}

TESTE(excluir_usuario_deve_excluir_os_personagens_dele) {
    CenarioFachada cenario;
    const int isac = cenario.criarUsuario("isac");
    const int bia = cenario.criarUsuario("bia");
    cenario.fachada.criarPersonagem(Personagem(isac, fichaValida("Thorin")));
    cenario.fachada.criarPersonagem(Personagem(isac, fichaValida("Lia")));
    cenario.fachada.criarPersonagem(Personagem(bia, fichaValida("Bruna")));

    VERIFICAR(cenario.fachada.excluirUsuario(isac));

    const auto restantes = cenario.fachada.listarPersonagens();
    VERIFICAR(restantes.size() == 1 && restantes[0].getIdDono() == bia);
    VERIFICAR(cenario.fachada.obterQuantidadeEntidades() == 2);
}
