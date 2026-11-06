#pragma once

#include <cstdint>
#include <functional>
#include <istream>
#include <ostream>
#include <string>

// Funcoes de leitura e escrita de arquivos binarios compartilhadas pelos
// repositorios em arquivo. Inteiros de tamanho fixo em little-endian
// (NF-POR-02). Toda falha de E/S vira FalhaArmazenamentoException.
namespace arquivo_binario {

void escreverUint32(std::ostream& saida, std::uint32_t valor);
void escreverInt32(std::ostream& saida, std::int32_t valor);
void escreverUint8(std::ostream& saida, std::uint8_t valor);
void escreverString(std::ostream& saida, const std::string& texto);

std::uint32_t lerUint32(std::istream& entrada);
std::int32_t lerInt32(std::istream& entrada);
std::uint8_t lerUint8(std::istream& entrada);
std::string lerString(std::istream& entrada, const std::string& caminho);

// Abre `caminho` e chama `ler`. Retorna false, sem chamar `ler`, se o arquivo
// ainda nao existir (primeira execucao). Converte leitura incompleta e dados
// extras no final em FalhaArmazenamentoException.
bool carregar(const std::string& caminho, const std::function<void(std::istream&)>& ler);

// Grava com `escrever` em `caminho`.tmp e depois substitui `caminho`, para
// nao deixar o arquivo pela metade se a gravacao falhar.
void gravar(const std::string& caminho, const std::function<void(std::ostream&)>& escrever);

}  // namespace arquivo_binario
