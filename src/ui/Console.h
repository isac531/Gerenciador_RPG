#pragma once

#include <string>

// Leitura de dados do console usada pelos menus.
namespace console {

// Le uma linha com um inteiro. Retorna false se a linha nao for um inteiro
// ou se a entrada terminou (confira std::cin para distinguir os casos).
bool lerInteiro(const std::string& mensagem, int& valor);

// Le uma linha de texto. Retorna false apenas se a entrada terminou.
bool lerLinha(const std::string& mensagem, std::string& texto);

}  // namespace console
