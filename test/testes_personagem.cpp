#include <string>

#include "../src/business/exception/Excecoes.h"
#include "../src/business/model/Personagem.h"
#include "../src/business/validation/RegrasFicha.h"
#include "TestRunner.h"

namespace {

Ficha fichaValida(const std::string& nome = "Thorin") {
    Ficha ficha;
    ficha.nome = nome;
    ficha.nivel = 3;
    ficha.pontosDeVida = 27;
    ficha.atributos = {16, 12, 14, 8, 10, 9};
    return ficha;
}

}  // namespace

// ---------- Ficha do personagem ----------

TESTE(modificador_deve_arredondar_para_baixo) {
    VERIFICAR(calcularModificador(16) == 3);
    VERIFICAR(calcularModificador(10) == 0);
    VERIFICAR(calcularModificador(9) == -1);
    VERIFICAR(calcularModificador(1) == -5);
}

TESTE(deve_aceitar_ficha_valida) {
    const Ficha ficha = fichaValida();
    VERIFICAR_NAO_LANCA(NomePersonagemRegra().validar(ficha));
    VERIFICAR_NAO_LANCA(NivelRegra().validar(ficha));
    VERIFICAR_NAO_LANCA(PontosDeVidaRegra().validar(ficha));
    VERIFICAR_NAO_LANCA(AtributosRegra().validar(ficha));
}

TESTE(deve_recusar_nome_de_personagem_vazio) {
    VERIFICAR_LANCA(NomePersonagemRegra().validar(fichaValida("   ")), NomePersonagemInvalidoException);
}

TESTE(deve_recusar_nome_de_personagem_com_31_caracteres) {
    VERIFICAR_LANCA(NomePersonagemRegra().validar(fichaValida(std::string(31, 'a'))),
                    NomePersonagemInvalidoException);
}

TESTE(deve_recusar_nivel_fora_de_1_a_20) {
    Ficha ficha = fichaValida();
    ficha.nivel = 0;
    VERIFICAR_LANCA(NivelRegra().validar(ficha), NivelInvalidoException);
    ficha.nivel = 21;
    VERIFICAR_LANCA(NivelRegra().validar(ficha), NivelInvalidoException);
}

TESTE(deve_recusar_pontos_de_vida_zerados) {
    Ficha ficha = fichaValida();
    ficha.pontosDeVida = 0;
    VERIFICAR_LANCA(PontosDeVidaRegra().validar(ficha), PontosDeVidaInvalidosException);
}

TESTE(deve_recusar_atributo_fora_de_1_a_30) {
    Ficha ficha = fichaValida();
    ficha.atributos.carisma = 31;
    VERIFICAR_LANCA(AtributosRegra().validar(ficha), AtributoInvalidoException);
    ficha.atributos.carisma = 0;
    VERIFICAR_LANCA(AtributosRegra().validar(ficha), AtributoInvalidoException);
}

TESTE(editar_ficha_deve_manter_id_e_dono) {
    const Personagem original(7, 2, fichaValida("Thorin"));
    const Personagem editado = original.comFicha(fichaValida("Thorin II"));
    VERIFICAR(editado.getId() == 7 && editado.getIdDono() == 2);
    VERIFICAR(editado.getFicha().nome == "Thorin II");
    VERIFICAR(original.getFicha().nome == "Thorin");
}
