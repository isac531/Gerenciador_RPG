#include "Console.h"

#include <iostream>
#include <sstream>

namespace console {

bool lerInteiro(const std::string& mensagem, int& valor) {
    std::string linha;
    if (!lerLinha(mensagem, linha)) {
        return false;
    }

    std::istringstream entrada(linha);
    char caractereExtra;
    return static_cast<bool>(entrada >> valor) && !(entrada >> caractereExtra);
}

bool lerLinha(const std::string& mensagem, std::string& texto) {
    std::cout << mensagem;
    return static_cast<bool>(std::getline(std::cin, texto));
}

}  // namespace console
