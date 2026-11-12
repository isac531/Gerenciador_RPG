#include <fstream>

#include "../src/business/exception/Excecoes.h"
#include "../src/business/model/RegistroAcesso.h"
#include "../src/infra/RepositorioAcessoArquivoBinario.h"
#include "../src/infra/RepositorioAcessoMemoria.h"
#include "ArquivoTemporario.h"
#include "TestRunner.h"

namespace {

// 11/11/2026 12:00:00 UTC
constexpr std::int64_t kInstante = 1794398400;

}  // namespace

// ---------- Repository de registros de acesso ----------

TESTE(repositorio_acesso_memoria_deve_atribuir_ids_em_sequencia) {
    RepositorioAcessoMemoria repositorio;
    VERIFICAR(repositorio.quantidade() == 0);
    VERIFICAR(repositorio.adicionar(RegistroAcesso("isac", 1, kInstante, true)) == 1);
    VERIFICAR(repositorio.adicionar(RegistroAcesso("bia", SEM_ID, kInstante + 5, false)) == 2);
    VERIFICAR(repositorio.quantidade() == 2);
    VERIFICAR(repositorio.listarTodos()[1].getLogin() == "bia");
}

TESTE(repositorio_acesso_deve_listar_por_login) {
    RepositorioAcessoMemoria repositorio;
    repositorio.adicionar(RegistroAcesso("isac", 1, kInstante, true));
    repositorio.adicionar(RegistroAcesso("bia", 2, kInstante + 1, true));
    repositorio.adicionar(RegistroAcesso("isac", 1, kInstante + 2, false));

    const auto doIsac = repositorio.listarPorLogin("isac");
    VERIFICAR(doIsac.size() == 2);
    VERIFICAR(doIsac[0].foiSucesso());
    VERIFICAR(!doIsac[1].foiSucesso());
    VERIFICAR(repositorio.listarPorLogin("ninguem").empty());
}

TESTE(repositorio_acesso_binario_deve_persistir_entre_instancias) {
    ArquivoTemporario arquivo("teste_acessos.dat");
    {
        RepositorioAcessoArquivoBinario repositorio(arquivo.caminho());
        repositorio.adicionar(RegistroAcesso("isac", 1, kInstante, true));
        repositorio.adicionar(RegistroAcesso("bia", SEM_ID, kInstante + 60, false));
    }

    RepositorioAcessoArquivoBinario reaberto(arquivo.caminho());
    const auto registros = reaberto.listarTodos();
    VERIFICAR(registros.size() == 2);
    VERIFICAR(registros[0].getId() == 1);
    VERIFICAR(registros[0].getLogin() == "isac");
    VERIFICAR(registros[0].getIdUsuario() == 1);
    VERIFICAR(registros[0].getDataHora() == kInstante);
    VERIFICAR(registros[0].foiSucesso());
    VERIFICAR(registros[1].getIdUsuario() == SEM_ID);
    VERIFICAR(!registros[1].foiSucesso());
    VERIFICAR(reaberto.adicionar(RegistroAcesso("isac", 1, kInstante + 120, true)) == 3);
}

TESTE(arquivo_de_acessos_corrompido_deve_lancar_falha_armazenamento) {
    ArquivoTemporario arquivo("teste_acessos_corrompido.dat");
    {
        std::ofstream(arquivo.caminho(), std::ios::binary) << "lixo";
    }
    VERIFICAR_LANCA(RepositorioAcessoArquivoBinario{arquivo.caminho()},
                    FalhaArmazenamentoException);
}
