#include "RepositorioAcessoArquivoBinario.h"

#include <cstdint>
#include <utility>

#include "../business/exception/Excecoes.h"
#include "ArquivoBinario.h"

using namespace arquivo_binario;

namespace {

void escreverRegistro(std::ostream& saida, const RegistroAcesso& registro) {
    escreverInt32(saida, registro.getId());
    escreverString(saida, registro.getLogin());
    escreverInt32(saida, registro.getIdUsuario());
    escreverInt64(saida, registro.getDataHora());
    escreverUint8(saida, registro.foiSucesso() ? 1 : 0);
}

RegistroAcesso lerRegistro(std::istream& entrada, const std::string& caminho) {
    const int id = lerInt32(entrada);
    std::string login = lerString(entrada, caminho);
    const int idUsuario = lerInt32(entrada);
    const std::int64_t dataHora = lerInt64(entrada);
    const std::uint8_t sucesso = lerUint8(entrada);
    if (sucesso > 1) {
        throw FalhaArmazenamentoException(
            "Arquivo " + caminho + " corrompido: resultado de acesso invalido.");
    }
    return RegistroAcesso(id, std::move(login), idUsuario, dataHora, sucesso == 1);
}

}  // namespace

RepositorioAcessoArquivoBinario::RepositorioAcessoArquivoBinario(std::string caminho)
    : caminho_(std::move(caminho)) {
    carregar();
}

void RepositorioAcessoArquivoBinario::carregar() {
    Estado carregado;
    const bool existia = arquivo_binario::carregar(caminho_, [&](std::istream& arquivo) {
        carregado.proximoId = lerInt32(arquivo);
        const std::uint32_t quantidade = lerUint32(arquivo);
        for (std::uint32_t i = 0; i < quantidade; ++i) {
            RegistroAcesso registro = lerRegistro(arquivo, caminho_);
            const int id = registro.getId();
            if (id <= 0 || id >= carregado.proximoId ||
                !carregado.registros.emplace(id, std::move(registro)).second) {
                throw FalhaArmazenamentoException(
                    "Arquivo " + caminho_ + " corrompido: ID invalido ou duplicado.");
            }
        }
    });
    if (existia) {
        estado_ = std::move(carregado);
    }
}

void RepositorioAcessoArquivoBinario::gravar() const {
    arquivo_binario::gravar(caminho_, [this](std::ostream& arquivo) {
        escreverInt32(arquivo, estado_.proximoId);
        escreverUint32(arquivo, static_cast<std::uint32_t>(estado_.registros.size()));
        for (const auto& entrada : estado_.registros) {
            escreverRegistro(arquivo, entrada.second);
        }
    });
}

int RepositorioAcessoArquivoBinario::adicionar(const RegistroAcesso& registro) {
    const Estado anterior = estado_;
    const int id = RepositorioAcessoMemoria::adicionar(registro);
    try {
        gravar();
    } catch (...) {
        estado_ = anterior;
        throw;
    }
    return id;
}
