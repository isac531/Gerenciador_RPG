#include <cstdio>
#include <optional>
#include <string>

#include "../src/business/control/ControlePersonagem.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/business/memento/HistoricoEdicaoPersonagem.h"
#include "../src/business/model/Personagem.h"
#include "../src/infra/RepositorioMemoria.h"
#include "../src/infra/RepositorioPersonagemArquivoBinario.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "TestRunner.h"

namespace {

Ficha fichaNivel(int nivel, int pontosDeVida) {
    return Ficha{"Thorin", nivel, pontosDeVida, Atributos{}};
}

// Controle de personagens ligado a repositorios novos em memoria.
struct CenarioMemento {
    RepositorioMemoria usuarios;
    RepositorioPersonagemMemoria personagens;
    ControlePersonagem controle{personagens, usuarios};
    int dono = usuarios.adicionar(Usuario("isac", "Senha@123", Papel::MESTRE));

    int criarThorin() { return controle.criar(Personagem(dono, fichaNivel(3, 27))); }
};

}  // namespace

// ---------- Memento: Personagem (Originator) e MementoPersonagem ----------

TESTE(memento_deve_guardar_o_id_do_personagem) {
    const Personagem personagem(1, 7, fichaNivel(3, 27));
    VERIFICAR(personagem.criarMemento().getIdPersonagem() == 1);
}

TESTE(restaurar_deve_voltar_a_ficha_do_memento_mantendo_id_e_dono) {
    const Personagem original(1, 7, fichaNivel(3, 27));
    const MementoPersonagem memento = original.criarMemento();
    const Personagem editado = original.comFicha(fichaNivel(4, 35));

    const Personagem restaurado = editado.restaurar(memento);

    VERIFICAR(restaurado.getId() == 1);
    VERIFICAR(restaurado.getIdDono() == 7);
    VERIFICAR(restaurado.getFicha().nivel == 3);
    VERIFICAR(restaurado.getFicha().pontosDeVida == 27);
}

TESTE(memento_nao_deve_mudar_quando_o_personagem_e_editado) {
    const Personagem original(1, 7, fichaNivel(3, 27));
    const MementoPersonagem memento = original.criarMemento();
    const Personagem editado = original.comFicha(fichaNivel(4, 35)).comFicha(fichaNivel(5, 40));
    VERIFICAR(editado.restaurar(memento).getFicha().nivel == 3);
}

TESTE(restaurar_memento_de_outro_personagem_deve_lancar_id_invalido) {
    const MementoPersonagem doPersonagem2 = Personagem(2, 7, fichaNivel(3, 27)).criarMemento();
    const Personagem personagem1(1, 7, fichaNivel(4, 35));
    VERIFICAR_LANCA(personagem1.restaurar(doPersonagem2), IdInvalidoException);
}

// ---------- Caretaker: HistoricoEdicaoPersonagem ----------

TESTE(historico_deve_comecar_vazio) {
    const HistoricoEdicaoPersonagem historico;
    VERIFICAR(historico.vazio());
    VERIFICAR(!historico.ultimo().has_value());
}

TESTE(historico_deve_guardar_apenas_a_ultima_edicao) {
    HistoricoEdicaoPersonagem historico;
    historico.guardar(Personagem(1, 7, fichaNivel(3, 27)).criarMemento());
    historico.guardar(Personagem(2, 7, fichaNivel(5, 40)).criarMemento());
    VERIFICAR(historico.ultimo()->getIdPersonagem() == 2);
}

TESTE(historico_descartado_deve_ficar_vazio) {
    HistoricoEdicaoPersonagem historico;
    historico.guardar(Personagem(1, 7, fichaNivel(3, 27)).criarMemento());
    historico.descartar();
    VERIFICAR(historico.vazio());
}

// ---------- ControlePersonagem: edicao devolve o memento; restaurar grava ----------

TESTE(editar_ficha_deve_devolver_o_memento_do_estado_anterior) {
    CenarioMemento cenario;
    const int id = cenario.criarThorin();

    const MementoPersonagem anterior = cenario.controle.editarFicha(id, fichaNivel(4, 35));
    VERIFICAR(anterior.getIdPersonagem() == id);

    const std::optional<Personagem> restaurado = cenario.controle.restaurar(anterior);
    VERIFICAR(restaurado.has_value());
    VERIFICAR(cenario.controle.buscar(id)->getFicha().nivel == 3);
    VERIFICAR(cenario.controle.buscar(id)->getIdDono() == cenario.dono);
}

TESTE(restaurar_personagem_excluido_deve_retornar_vazio) {
    CenarioMemento cenario;
    const int id = cenario.criarThorin();
    const MementoPersonagem anterior = cenario.controle.editarFicha(id, fichaNivel(4, 35));
    cenario.controle.excluir(id);

    VERIFICAR(!cenario.controle.restaurar(anterior).has_value());
    VERIFICAR(cenario.controle.quantidade() == 0);
}

TESTE(restaurar_deve_persistir_no_arquivo_binario) {
    const std::string caminho = "teste_memento_personagens.dat";
    std::remove(caminho.c_str());
    RepositorioMemoria usuarios;
    const int dono = usuarios.adicionar(Usuario("isac", "Senha@123", Papel::MESTRE));
    int id;
    {
        RepositorioPersonagemArquivoBinario personagens(caminho);
        ControlePersonagem controle(personagens, usuarios);
        id = controle.criar(Personagem(dono, fichaNivel(3, 27)));
        const MementoPersonagem anterior = controle.editarFicha(id, fichaNivel(4, 35));
        controle.restaurar(anterior);
    }
    RepositorioPersonagemArquivoBinario recarregado(caminho);
    VERIFICAR(recarregado.buscar(id)->getFicha().nivel == 3);
    std::remove(caminho.c_str());
}
