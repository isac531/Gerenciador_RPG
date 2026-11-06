#include "ArquivoBinario.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <limits>

#include "../business/exception/Excecoes.h"

namespace arquivo_binario {

namespace {

// Limite de seguranca para strings lidas do arquivo: evita alocar memoria
// demais quando o tamanho gravado estiver corrompido.
constexpr std::uint32_t kTamanhoMaximoString = 64 * 1024;

}  // namespace

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

bool carregar(const std::string& caminho, const std::function<void(std::istream&)>& ler) {
    std::error_code erro;
    if (!std::filesystem::exists(caminho, erro)) {
        if (erro) {
            throw FalhaArmazenamentoException(
                "Nao foi possivel verificar o arquivo " + caminho + ".");
        }
        return false;
    }

    std::ifstream arquivo;
    arquivo.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        arquivo.open(caminho, std::ios::in | std::ios::binary);
    } catch (const std::ios_base::failure&) {
        throw FalhaArmazenamentoException(
            "Nao foi possivel abrir o arquivo " + caminho + " para leitura.");
    }

    try {
        ler(arquivo);
        if (arquivo.peek() != std::ifstream::traits_type::eof()) {
            throw FalhaArmazenamentoException(
                "Arquivo " + caminho + " corrompido: dados extras no final.");
        }
    } catch (const std::ios_base::failure&) {
        if (arquivo.eof()) {
            throw FalhaArmazenamentoException(
                "Arquivo " + caminho + " corrompido ou truncado.");
        }
        throw FalhaArmazenamentoException("Erro de leitura no arquivo " + caminho + ".");
    }
    return true;
}

void gravar(const std::string& caminho, const std::function<void(std::ostream&)>& escrever) {
    const std::string temporario = caminho + ".tmp";
    try {
        std::ofstream arquivo;
        arquivo.exceptions(std::ios::failbit | std::ios::badbit);
        arquivo.open(temporario, std::ios::out | std::ios::binary | std::ios::trunc);
        escrever(arquivo);
        arquivo.close();
    } catch (const std::ios_base::failure&) {
        std::remove(temporario.c_str());
        throw FalhaArmazenamentoException("Nao foi possivel gravar o arquivo " + caminho + ".");
    }

    if (std::rename(temporario.c_str(), caminho.c_str()) != 0) {
        // No Windows, rename falha se o destino ja existir.
        std::remove(caminho.c_str());
        if (std::rename(temporario.c_str(), caminho.c_str()) != 0) {
            std::remove(temporario.c_str());
            throw FalhaArmazenamentoException(
                "Nao foi possivel substituir o arquivo " + caminho + ".");
        }
    }
}

}  // namespace arquivo_binario
