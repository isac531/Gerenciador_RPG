#include "RepositorioPersonagemArquivoBinario.h"

#include <cstdint>
#include <utility>

#include "../business/exception/Excecoes.h"
#include "ArquivoBinario.h"

using namespace arquivo_binario;

namespace {

void escreverPersonagem(std::ostream& saida, const Personagem& personagem) {
    const Ficha& ficha = personagem.getFicha();
    const Atributos& atributos = ficha.atributos;
    escreverInt32(saida, personagem.getId());
    escreverInt32(saida, personagem.getIdDono());
    escreverString(saida, ficha.nome);
    for (int valor : {ficha.nivel, ficha.pontosDeVida, atributos.forca, atributos.destreza,
                      atributos.constituicao, atributos.inteligencia, atributos.sabedoria,
                      atributos.carisma}) {
        escreverInt32(saida, valor);
    }
}

Personagem lerPersonagem(std::istream& entrada, const std::string& caminho) {
    const int id = lerInt32(entrada);
    const int idDono = lerInt32(entrada);
    Ficha ficha;
    ficha.nome = lerString(entrada, caminho);
    ficha.nivel = lerInt32(entrada);
    ficha.pontosDeVida = lerInt32(entrada);
    Atributos& atributos = ficha.atributos;
    for (int* valor : {&atributos.forca, &atributos.destreza, &atributos.constituicao,
                       &atributos.inteligencia, &atributos.sabedoria, &atributos.carisma}) {
        *valor = lerInt32(entrada);
    }
    return Personagem(id, idDono, std::move(ficha));
}

}  // namespace

RepositorioPersonagemArquivoBinario::RepositorioPersonagemArquivoBinario(std::string caminho)
    : caminho_(std::move(caminho)) {
    carregar();
}

void RepositorioPersonagemArquivoBinario::carregar() {
    Estado carregado;
    const bool existia = arquivo_binario::carregar(caminho_, [&](std::istream& arquivo) {
        carregado.proximoId = lerInt32(arquivo);
        const std::uint32_t quantidade = lerUint32(arquivo);
        for (std::uint32_t i = 0; i < quantidade; ++i) {
            Personagem personagem = lerPersonagem(arquivo, caminho_);
            const int id = personagem.getId();
            if (id <= 0 || id >= carregado.proximoId ||
                !carregado.personagens.emplace(id, std::move(personagem)).second) {
                throw FalhaArmazenamentoException(
                    "Arquivo " + caminho_ + " corrompido: ID invalido ou duplicado.");
            }
        }
    });
    if (existia) {
        estado_ = std::move(carregado);
    }
}

void RepositorioPersonagemArquivoBinario::gravar() const {
    arquivo_binario::gravar(caminho_, [this](std::ostream& arquivo) {
        escreverInt32(arquivo, estado_.proximoId);
        escreverUint32(arquivo, static_cast<std::uint32_t>(estado_.personagens.size()));
        for (const auto& entrada : estado_.personagens) {
            escreverPersonagem(arquivo, entrada.second);
        }
    });
}

void RepositorioPersonagemArquivoBinario::persistirOuDesfazer(const Estado& anterior) {
    try {
        gravar();
    } catch (...) {
        estado_ = anterior;
        throw;
    }
}

int RepositorioPersonagemArquivoBinario::adicionar(const Personagem& personagem) {
    const Estado anterior = estado_;
    const int id = RepositorioPersonagemMemoria::adicionar(personagem);
    persistirOuDesfazer(anterior);
    return id;
}

bool RepositorioPersonagemArquivoBinario::atualizar(const Personagem& personagem) {
    const Estado anterior = estado_;
    if (!RepositorioPersonagemMemoria::atualizar(personagem)) {
        return false;
    }
    persistirOuDesfazer(anterior);
    return true;
}

bool RepositorioPersonagemArquivoBinario::remover(int id) {
    const Estado anterior = estado_;
    if (!RepositorioPersonagemMemoria::remover(id)) {
        return false;
    }
    persistirOuDesfazer(anterior);
    return true;
}
