#include "RepositorioArquivoBinario.h"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <limits>
#include <utility>

#include "../business/exception/Excecoes.h"

namespace {

// Limite de seguranca para strings lidas do arquivo: evita alocar memoria
// demais quando o tamanho gravado estiver corrompido.
constexpr std::uint32_t kTamanhoMaximoString = 64 * 1024;

void escreverUint32(std::ostream& saida, std::uint32_t valor) {
    char bytes[4];
    for (int i = 0; i < 4; ++i) {
        bytes[i] = static_cast<char>((valor >> (8 * i)) & 0xFFu);
    }
    saida.write(bytes, sizeof(bytes));
}

void escreverInt32(std::ostream& saida, std::int32_t valor) {
    escreverUint32(saida, static_cast<std::uint32_t>(valor));
}

void escreverUint8(std::ostream& saida, std::uint8_t valor) {
    saida.put(static_cast<char>(valor));
}

void escreverString(std::ostream& saida, const std::string& texto) {
    escreverUint32(saida, static_cast<std::uint32_t>(texto.size()));
    saida.write(texto.data(), static_cast<std::streamsize>(texto.size()));
}

std::uint32_t lerUint32(std::istream& entrada) {
    unsigned char bytes[4];
    entrada.read(reinterpret_cast<char*>(bytes), sizeof(bytes));
    std::uint32_t valor = 0;
    for (int i = 0; i < 4; ++i) {
        valor |= static_cast<std::uint32_t>(bytes[i]) << (8 * i);
    }
    return valor;
}

std::int32_t lerInt32(std::istream& entrada) {
    const std::uint32_t bruto = lerUint32(entrada);
    if (bruto <= static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())) {
        return static_cast<std::int32_t>(bruto);
    }
    // Complemento de dois sem depender de conversao definida pela implementacao.
    return static_cast<std::int32_t>(bruto - 0x80000000u) + std::numeric_limits<std::int32_t>::min();
}

std::uint8_t lerUint8(std::istream& entrada) {
    char byte;
    entrada.get(byte);
    return static_cast<std::uint8_t>(byte);
}

std::string lerString(std::istream& entrada, const std::string& caminho) {
    const std::uint32_t tamanho = lerUint32(entrada);
    if (tamanho > kTamanhoMaximoString) {
        throw FalhaArmazenamentoException(
            "Arquivo " + caminho + " corrompido: tamanho de texto invalido.");
    }
    std::string texto(tamanho, '\0');
    if (tamanho > 0) {
        entrada.read(&texto[0], static_cast<std::streamsize>(tamanho));
    }
    return texto;
}

}  // namespace

RepositorioArquivoBinario::RepositorioArquivoBinario(std::string caminho)
    : caminho_(std::move(caminho)) {
    carregar();
}

const std::string& RepositorioArquivoBinario::getCaminho() const {
    return caminho_;
}

void RepositorioArquivoBinario::carregar() {
    std::error_code erro;
    if (!std::filesystem::exists(caminho_, erro)) {
        if (erro) {
            throw FalhaArmazenamentoException(
                "Nao foi possivel verificar o arquivo " + caminho_ + ".");
        }
        return;  // Primeira execucao: comeca vazio e cria o arquivo ao gravar.
    }

    std::ifstream arquivo;
    arquivo.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        arquivo.open(caminho_, std::ios::in | std::ios::binary);
    } catch (const std::ios_base::failure&) {
        throw FalhaArmazenamentoException(
            "Nao foi possivel abrir o arquivo " + caminho_ + " para leitura.");
    }

    std::map<int, Usuario> carregados;
    try {
        const std::uint32_t quantidade = lerUint32(arquivo);
        for (std::uint32_t i = 0; i < quantidade; ++i) {
            const std::int32_t id = lerInt32(arquivo);
            std::string login = lerString(arquivo, caminho_);
            std::string senha = lerString(arquivo, caminho_);
            Papel papel;
            if (!papelDeCodigo(lerUint8(arquivo), papel)) {
                throw FalhaArmazenamentoException(
                    "Arquivo " + caminho_ + " corrompido: papel invalido.");
            }
            if (id <= 0 ||
                !carregados.emplace(id, Usuario(id, std::move(login), std::move(senha), papel)).second) {
                throw FalhaArmazenamentoException(
                    "Arquivo " + caminho_ + " corrompido: ID invalido ou duplicado.");
            }
        }
        if (arquivo.peek() != std::ifstream::traits_type::eof()) {
            throw FalhaArmazenamentoException(
                "Arquivo " + caminho_ + " corrompido: dados extras no final.");
        }
    } catch (const std::ios_base::failure&) {
        if (arquivo.eof()) {
            throw FalhaArmazenamentoException(
                "Arquivo " + caminho_ + " corrompido ou truncado.");
        }
        throw FalhaArmazenamentoException(
            "Erro de leitura no arquivo " + caminho_ + ".");
    }

    usuarios_ = std::move(carregados);
}

void RepositorioArquivoBinario::gravar() const {
    // Grava em um arquivo temporario e depois o renomeia, para nao deixar
    // usuarios.dat pela metade se a gravacao falhar.
    const std::string temporario = caminho_ + ".tmp";
    try {
        std::ofstream arquivo;
        arquivo.exceptions(std::ios::failbit | std::ios::badbit);
        arquivo.open(temporario, std::ios::out | std::ios::binary | std::ios::trunc);

        escreverUint32(arquivo, static_cast<std::uint32_t>(usuarios_.size()));
        for (const auto& entrada : usuarios_) {
            const Usuario& usuario = entrada.second;
            escreverInt32(arquivo, usuario.getId());
            escreverString(arquivo, usuario.getLogin());
            escreverString(arquivo, usuario.getSenha());
            escreverUint8(arquivo, static_cast<std::uint8_t>(usuario.getPapel()));
        }
        arquivo.close();
    } catch (const std::ios_base::failure&) {
        std::remove(temporario.c_str());
        throw FalhaArmazenamentoException(
            "Nao foi possivel gravar o arquivo " + caminho_ + ".");
    }

    if (std::rename(temporario.c_str(), caminho_.c_str()) != 0) {
        // No Windows, rename falha se o destino ja existir.
        std::remove(caminho_.c_str());
        if (std::rename(temporario.c_str(), caminho_.c_str()) != 0) {
            std::remove(temporario.c_str());
            throw FalhaArmazenamentoException(
                "Nao foi possivel substituir o arquivo " + caminho_ + ".");
        }
    }
}

bool RepositorioArquivoBinario::adicionar(const Usuario& usuario) {
    if (!usuarios_.emplace(usuario.getId(), usuario).second) {
        return false;
    }
    try {
        gravar();
    } catch (...) {
        usuarios_.erase(usuario.getId());
        throw;
    }
    return true;
}

bool RepositorioArquivoBinario::remover(int id) {
    const auto encontrado = usuarios_.find(id);
    if (encontrado == usuarios_.end()) {
        return false;
    }

    const Usuario removido = encontrado->second;
    usuarios_.erase(encontrado);
    try {
        gravar();
    } catch (...) {
        usuarios_.emplace(id, removido);
        throw;
    }
    return true;
}

bool RepositorioArquivoBinario::existe(int id) const {
    return usuarios_.count(id) > 0;
}

std::optional<Usuario> RepositorioArquivoBinario::buscar(int id) const {
    const auto encontrado = usuarios_.find(id);
    if (encontrado == usuarios_.end()) {
        return std::nullopt;
    }
    return encontrado->second;
}

std::vector<Usuario> RepositorioArquivoBinario::listarTodos() const {
    std::vector<Usuario> todos;
    todos.reserve(usuarios_.size());
    for (const auto& entrada : usuarios_) {
        todos.push_back(entrada.second);
    }
    return todos;
}
