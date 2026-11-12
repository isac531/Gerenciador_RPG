#include <fstream>
#include <memory>

#include "../src/business/exception/Excecoes.h"
#include "../src/business/model/Personagem.h"
#include "../src/business/model/RegistroAcesso.h"
#include "../src/business/model/Usuario.h"
#include "../src/business/repository/FabricaRepositorio.h"
#include "../src/infra/EscolhaFabrica.h"
#include "../src/infra/FabricaArquivoBinario.h"
#include "../src/infra/FabricaMemoria.h"
#include "ArquivoTemporario.h"
#include "TestRunner.h"

namespace {

Usuario usuarioExemplo() {
    return Usuario("isac", "Dado#2026", Papel::MESTRE);
}

Ficha fichaExemplo() {
    Ficha ficha;
    ficha.nome = "Thorin";
    ficha.nivel = 3;
    ficha.pontosDeVida = 27;
    ficha.atributos = {16, 12, 14, 8, 10, 9};
    return ficha;
}

}  // namespace

// ---------- Factory Method: escolha da fabrica ----------

TESTE(escolher_fabrica_deve_devolver_a_fabrica_do_tipo_pedido) {
    VERIFICAR(escolherFabrica(TipoArmazenamento::MEMORIA)->tipo() == TipoArmazenamento::MEMORIA);
    VERIFICAR(escolherFabrica(TipoArmazenamento::ARQUIVO_BINARIO)->tipo() ==
              TipoArmazenamento::ARQUIVO_BINARIO);
}

// ---------- Factory Method: fabrica em memoria ----------

TESTE(fabrica_memoria_deve_criar_repositorios_vazios) {
    FabricaMemoria fabrica;
    auto usuarios = fabrica.criarRepositorioUsuarios();
    auto personagens = fabrica.criarRepositorioPersonagens();
    VERIFICAR(usuarios != nullptr);
    VERIFICAR(personagens != nullptr);
    auto acessos = fabrica.criarRepositorioAcessos();
    VERIFICAR(usuarios->quantidade() == 0);
    VERIFICAR(personagens->quantidade() == 0);
    VERIFICAR(acessos != nullptr);
    VERIFICAR(acessos->quantidade() == 0);
}

TESTE(fabrica_memoria_deve_criar_repositorios_independentes) {
    FabricaMemoria fabrica;
    auto primeiro = fabrica.criarRepositorioUsuarios();
    auto segundo = fabrica.criarRepositorioUsuarios();
    primeiro->adicionar(usuarioExemplo());
    VERIFICAR(primeiro->quantidade() == 1);
    VERIFICAR(segundo->quantidade() == 0);
}

// ---------- Factory Method: fabrica em arquivo binario ----------

TESTE(fabrica_arquivo_deve_criar_repositorios_que_persistem) {
    ArquivoTemporario arquivoUsuarios("fabrica_usuarios.dat");
    ArquivoTemporario arquivoPersonagens("fabrica_personagens.dat");
    ArquivoTemporario arquivoAcessos("fabrica_acessos.dat");
    FabricaArquivoBinario fabrica(arquivoUsuarios.caminho(), arquivoPersonagens.caminho(),
                                  arquivoAcessos.caminho());

    int idDono = 0;
    {
        Repositorios repositorios = fabrica.criarRepositorios();
        idDono = repositorios.usuarios->adicionar(usuarioExemplo());
        repositorios.personagens->adicionar(Personagem(idDono, fichaExemplo()));
        repositorios.acessos->adicionar(RegistroAcesso("isac", idDono, 1794398400, true));
    }

    Repositorios reaberto = fabrica.criarRepositorios();
    VERIFICAR(reaberto.usuarios->quantidade() == 1);
    VERIFICAR(reaberto.personagens->quantidade() == 1);
    VERIFICAR(reaberto.acessos->quantidade() == 1);
    VERIFICAR(reaberto.usuarios->buscar(idDono)->getLogin() == "isac");
}

TESTE(fabrica_arquivo_deve_lancar_falha_com_arquivo_corrompido) {
    ArquivoTemporario arquivoUsuarios("fabrica_corrompido.dat");
    {
        std::ofstream(arquivoUsuarios.caminho(), std::ios::binary) << "lixo";
    }
    FabricaArquivoBinario fabrica(arquivoUsuarios.caminho(), "nao_usado.dat");
    VERIFICAR_LANCA(fabrica.criarRepositorioUsuarios(), FalhaArmazenamentoException);
}

// ---------- Factory Method: o cliente usa so a classe abstrata ----------

TESTE(criar_repositorios_deve_funcionar_por_ponteiro_para_a_classe_abstrata) {
    std::unique_ptr<FabricaRepositorio> fabrica = escolherFabrica(TipoArmazenamento::MEMORIA);
    Repositorios repositorios = fabrica->criarRepositorios();
    VERIFICAR(repositorios.usuarios != nullptr);
    VERIFICAR(repositorios.personagens != nullptr);
    VERIFICAR(repositorios.acessos != nullptr);
}
