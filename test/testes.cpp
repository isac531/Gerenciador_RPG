#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "../src/business/control/ControleCriarConta.h"
#include "../src/business/control/ControleUsuarios.h"
#include "../src/business/exception/Excecoes.h"
#include "../src/infra/RepositorioArquivoBinario.h"
#include "../src/infra/RepositorioMemoria.h"
#include "TestRunner.h"

namespace {

const std::string kSenhaValida = "Senha@123";

// Repositorio em memoria com os dois controles ligados a ele.
struct Cenario {
    RepositorioMemoria repositorio;
    ControleCriarConta criacao{repositorio};
    ControleUsuarios usuarios{repositorio};
};

std::unique_ptr<Cenario> novoControle() {
    return std::make_unique<Cenario>();
}

Usuario usuarioComLogin(const std::string& login) {
    return Usuario(1, login, kSenhaValida, Papel::JOGADOR);
}

Usuario usuarioComSenha(const std::string& senha) {
    return Usuario(1, "isac", senha, Papel::JOGADOR);
}

// Caminho temporario unico, apagado ao sair do escopo.
class ArquivoTemporario {
public:
    explicit ArquivoTemporario(const std::string& nome)
        : caminho_((std::filesystem::temp_directory_path() / nome).string()) {
        std::remove(caminho_.c_str());
    }
    ~ArquivoTemporario() { std::remove(caminho_.c_str()); }
    const std::string& caminho() const { return caminho_; }

private:
    std::string caminho_;
};

}  // namespace

// ---------- Regras de login ----------

TESTE(deve_recusar_login_vazio) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaLogin(usuarioComLogin("")), LoginVazioException);
}

TESTE(deve_recusar_login_so_com_espacos) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaLogin(usuarioComLogin("   ")), LoginVazioException);
}

TESTE(deve_recusar_login_com_13_caracteres) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaLogin(usuarioComLogin("abcdefghijklm")), LoginLongoException);
}

TESTE(deve_aceitar_login_com_12_caracteres) {
    auto controle = novoControle();
    VERIFICAR_NAO_LANCA(controle->criacao.validaLogin(usuarioComLogin("abcdefghijkl")));
}

TESTE(deve_recusar_login_com_numeros) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaLogin(usuarioComLogin("isac1")), LoginComNumeroException);
}

TESTE(deve_recusar_login_ja_existente) {
    auto controle = novoControle();
    controle->criacao.criarConta(Usuario(1, "isac", kSenhaValida, Papel::MESTRE));
    VERIFICAR_LANCA(controle->criacao.criarConta(Usuario(2, "isac", "Outra@123", Papel::JOGADOR)),
                    LoginEmUsoException);
    VERIFICAR(controle->usuarios.listar().size() == 1);
}

// ---------- Regras de senha ----------

TESTE(deve_recusar_senha_com_menos_de_8_caracteres) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaSenha(usuarioComSenha("Ab@1234")), SenhaTamanhoInvalidoException);
}

TESTE(deve_recusar_senha_com_mais_de_128_caracteres) {
    auto controle = novoControle();
    const std::string senha = "Aa1" + std::string(126, 'a');
    VERIFICAR(senha.size() == 129);
    VERIFICAR_LANCA(controle->criacao.validaSenha(usuarioComSenha(senha)), SenhaTamanhoInvalidoException);
}

TESTE(deve_recusar_senha_com_apenas_2_tipos_de_caractere) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaSenha(usuarioComSenha("senhasenha123")),
                    SenhaCaracteresInsuficientesException);
}

TESTE(deve_aceitar_senha_com_3_tipos_de_caractere) {
    auto controle = novoControle();
    VERIFICAR_NAO_LANCA(controle->criacao.validaSenha(usuarioComSenha("Senhasenha123")));
}

TESTE(deve_recusar_senha_igual_ao_login) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.validaSenha(Usuario(1, "Abc@abcd", "Abc@abcd", Papel::JOGADOR)),
                    SenhaIgualUsuarioException);
}

TESTE(mensagens_de_erro_nao_devem_conter_a_senha) {
    auto controle = novoControle();
    const std::string senha = "segredo";
    try {
        controle->criacao.criarConta(Usuario(1, "isac", senha, Papel::JOGADOR));
        FALHAR("cadastro deveria falhar");
    } catch (const SenhaInvalidaException& e) {
        VERIFICAR(std::string(e.what()).find(senha) == std::string::npos);
    }
}

// ---------- Cadastro e listagem ----------

TESTE(deve_recusar_cadastro_com_id_duplicado) {
    auto controle = novoControle();
    controle->criacao.criarConta(Usuario(1, "isac", kSenhaValida, Papel::MESTRE));
    VERIFICAR_LANCA(controle->criacao.criarConta(Usuario(1, "bia", kSenhaValida, Papel::JOGADOR)),
                    IdInvalidoException);
    VERIFICAR(controle->usuarios.listar().size() == 1);
}

TESTE(listar_deve_retornar_os_usuarios_cadastrados) {
    auto controle = novoControle();
    controle->criacao.criarConta(Usuario(2, "bia", kSenhaValida, Papel::JOGADOR));
    controle->criacao.criarConta(Usuario(1, "isac", kSenhaValida, Papel::MESTRE));

    const auto usuarios = controle->usuarios.listar();
    VERIFICAR(usuarios.size() == 2);
    VERIFICAR(usuarios[0].getId() == 1 && usuarios[0].getLogin() == "isac");
    VERIFICAR(usuarios[0].getPapel() == Papel::MESTRE);
    VERIFICAR(usuarios[1].getId() == 2 && usuarios[1].getLogin() == "bia");
    VERIFICAR(usuarios[1].getPapel() == Papel::JOGADOR);
}

TESTE(deve_recusar_id_nao_positivo) {
    auto controle = novoControle();
    VERIFICAR_LANCA(controle->criacao.criarConta(Usuario(0, "isac", kSenhaValida, Papel::MESTRE)),
                    IdInvalidoException);
    VERIFICAR(controle->usuarios.listar().empty());
}

TESTE(listar_sem_usuarios_deve_retornar_vazio) {
    auto controle = novoControle();
    VERIFICAR(controle->usuarios.listar().empty());
}

TESTE(deve_excluir_usuario_existente) {
    auto controle = novoControle();
    controle->criacao.criarConta(Usuario(2, "bia", kSenhaValida, Papel::JOGADOR));
    VERIFICAR(controle->usuarios.excluirConta(2));
    VERIFICAR(controle->usuarios.listar().empty());
}

TESTE(excluir_id_inexistente_deve_retornar_falso) {
    auto controle = novoControle();
    controle->criacao.criarConta(Usuario(1, "isac", kSenhaValida, Papel::MESTRE));
    VERIFICAR(!controle->usuarios.excluirConta(9));
    VERIFICAR(controle->usuarios.listar().size() == 1);
}

// ---------- Repositorios ----------

TESTE(repositorio_memoria_deve_comecar_vazio) {
    RepositorioMemoria repositorio;
    VERIFICAR(repositorio.listarTodos().empty());
    VERIFICAR(!repositorio.existe(1));
}

TESTE(repositorio_binario_deve_persistir_entre_instancias) {
    ArquivoTemporario arquivo("rpg_teste_persistencia.dat");
    {
        RepositorioArquivoBinario repositorio(arquivo.caminho());
        VERIFICAR(repositorio.listarTodos().empty());
        VERIFICAR(repositorio.adicionar(Usuario(1, "isac", kSenhaValida, Papel::MESTRE)));
        VERIFICAR(repositorio.adicionar(Usuario(2, "bia", "Outra@123", Papel::JOGADOR)));
        VERIFICAR(repositorio.remover(2));
        VERIFICAR(repositorio.adicionar(Usuario(3, "ana", "Mais@1234", Papel::JOGADOR)));
    }

    RepositorioArquivoBinario reaberto(arquivo.caminho());
    const auto usuarios = reaberto.listarTodos();
    VERIFICAR(usuarios.size() == 2);
    VERIFICAR(usuarios[0].getId() == 1 && usuarios[0].getLogin() == "isac");
    VERIFICAR(usuarios[0].getSenha() == kSenhaValida && usuarios[0].getPapel() == Papel::MESTRE);
    VERIFICAR(usuarios[1].getId() == 3 && usuarios[1].getLogin() == "ana");
    VERIFICAR(usuarios[1].getPapel() == Papel::JOGADOR);
}

TESTE(arquivo_binario_truncado_deve_lancar_falha_armazenamento) {
    ArquivoTemporario arquivo("rpg_teste_truncado.dat");
    {
        RepositorioArquivoBinario repositorio(arquivo.caminho());
        repositorio.adicionar(Usuario(1, "isac", kSenhaValida, Papel::MESTRE));
    }
    std::filesystem::resize_file(arquivo.caminho(), 7);

    VERIFICAR_LANCA(RepositorioArquivoBinario{arquivo.caminho()}, FalhaArmazenamentoException);
}

TESTE(arquivo_binario_corrompido_deve_lancar_falha_armazenamento) {
    ArquivoTemporario arquivo("rpg_teste_corrompido.dat");
    {
        std::ofstream saida(arquivo.caminho(), std::ios::binary);
        saida << "isto nao e um arquivo de usuarios";
    }

    VERIFICAR_LANCA(RepositorioArquivoBinario{arquivo.caminho()}, FalhaArmazenamentoException);
}

int main() {
    return executarTestes();
}
