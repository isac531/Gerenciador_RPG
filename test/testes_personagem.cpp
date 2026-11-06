#include <fstream>
#include <string>

#include "../src/business/exception/Excecoes.h"
#include "../src/business/model/Personagem.h"
#include "../src/business/validation/RegrasFicha.h"
#include "../src/infra/RepositorioPersonagemArquivoBinario.h"
#include "../src/infra/RepositorioPersonagemMemoria.h"
#include "ArquivoTemporario.h"
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

// ---------- Repositorios de personagem ----------

TESTE(repositorio_personagem_memoria_deve_fazer_crud) {
    RepositorioPersonagemMemoria repositorio;
    const int id = repositorio.adicionar(Personagem(1, fichaValida()));
    VERIFICAR(id == 1);
    VERIFICAR(repositorio.quantidade() == 1);

    Ficha nova = fichaValida("Thorin II");
    nova.nivel = 4;
    VERIFICAR(repositorio.atualizar(repositorio.buscar(id)->comFicha(nova)));
    VERIFICAR(repositorio.buscar(id)->getFicha().nome == "Thorin II");
    VERIFICAR(repositorio.buscar(id)->getFicha().nivel == 4);

    VERIFICAR(repositorio.remover(id));
    VERIFICAR(!repositorio.buscar(id).has_value());
    VERIFICAR(!repositorio.remover(id));
    VERIFICAR(!repositorio.atualizar(Personagem(id, 1, nova)));
}

TESTE(repositorio_personagem_binario_deve_persistir_entre_instancias) {
    ArquivoTemporario arquivo("rpg_teste_personagens.dat");
    {
        RepositorioPersonagemArquivoBinario repositorio(arquivo.caminho());
        repositorio.adicionar(Personagem(1, fichaValida("Thorin")));
        const int idExcluido = repositorio.adicionar(Personagem(2, fichaValida("Lia")));
        repositorio.remover(idExcluido);
    }

    RepositorioPersonagemArquivoBinario reaberto(arquivo.caminho());
    const auto personagens = reaberto.listarTodos();
    VERIFICAR(personagens.size() == 1);
    const Personagem& thorin = personagens[0];
    VERIFICAR(thorin.getId() == 1 && thorin.getIdDono() == 1);
    VERIFICAR(thorin.getFicha().nome == "Thorin" && thorin.getFicha().pontosDeVida == 27);
    VERIFICAR(thorin.getFicha().atributos.forca == 16 && thorin.getFicha().atributos.carisma == 9);
    VERIFICAR(reaberto.adicionar(Personagem(1, fichaValida("Bruna"))) == 3);
}

TESTE(arquivo_de_personagens_corrompido_deve_lancar_falha_armazenamento) {
    ArquivoTemporario arquivo("rpg_teste_personagens_corrompido.dat");
    {
        std::ofstream saida(arquivo.caminho(), std::ios::binary);
        saida << "lixo";
    }
    VERIFICAR_LANCA(RepositorioPersonagemArquivoBinario{arquivo.caminho()},
                    FalhaArmazenamentoException);
}
