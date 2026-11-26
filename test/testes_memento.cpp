#include "../src/business/exception/Excecoes.h"
#include "../src/business/model/Personagem.h"
#include "TestRunner.h"

namespace {

Ficha fichaNivel(int nivel, int pontosDeVida) {
    return Ficha{"Thorin", nivel, pontosDeVida, Atributos{}};
}

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
