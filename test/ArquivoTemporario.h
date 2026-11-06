#pragma once

#include <cstdio>
#include <filesystem>
#include <string>

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
