#include "ControleCriarConta.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "../validation/RegrasLogin.h"
#include "../validation/RegrasSenha.h"

// Para criar uma nova regra: implemente IRegraValidacao e registre aqui.
// A ordem de registro define a ordem de execucao.
ControleCriarConta::ControleCriarConta() {
    regrasLogin_.push_back(std::make_unique<LoginVazioRegra>());
    regrasLogin_.push_back(std::make_unique<LoginLongoRegra>());
    regrasLogin_.push_back(std::make_unique<LoginComNumeroRegra>());

    regrasSenha_.push_back(std::make_unique<SenhaTamanhoRegra>());
    regrasSenha_.push_back(std::make_unique<SenhaIgualUsuarioRegra>());
    regrasSenha_.push_back(std::make_unique<SenhaCaracteresRegra>());

    carregarUsuarios();
}

void ControleCriarConta::carregarUsuarios() {
    std::ifstream arquivo("usuarios.txt");
    if (!arquivo) {
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) {
            continue;
        }

        std::istringstream registro(linha);
        int id;
        std::string nome;
        std::string senha;
        std::string dadoExtra;
        if (!(registro >> id >> std::quoted(nome) >> std::quoted(senha)) ||
            (registro >> dadoExtra)) {
            throw std::runtime_error("Arquivo usuarios.txt contem um registro invalido.");
        }
        if (id <= 0 || !usuarios_.emplace(id, Usuario(id, nome, senha)).second) {
            throw std::runtime_error("Arquivo usuarios.txt contem um ID invalido ou duplicado.");
        }
    }

    if (arquivo.bad()) {
        throw std::runtime_error("Nao foi possivel ler o arquivo usuarios.txt.");
    }
}

void ControleCriarConta::salvarUsuarios() const {
    std::ofstream arquivo("usuarios.txt", std::ios::trunc);
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel abrir usuarios.txt para gravacao.");
    }

    for (const auto& entrada : usuarios_) {
        arquivo << entrada.first << ' ' << std::quoted(entrada.second.getNome()) << ' '
                << std::quoted(entrada.second.getSenha()) << '\n';
    }

    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel gravar usuarios.txt.");
    }
}

void ControleCriarConta::executar(const Regras& regras, const Usuario& usuario) {
    for (const auto& regra : regras) {
        regra->validar(usuario);
    }
}

void ControleCriarConta::validaLogin(const Usuario& usuario) const {
    executar(regrasLogin_, usuario);
}

void ControleCriarConta::validaSenha(const Usuario& usuario) const {
    executar(regrasSenha_, usuario);
}

void ControleCriarConta::criarConta(const Usuario& usuario) {
    if (usuario.getId() <= 0) {
        throw std::invalid_argument("O ID deve ser um numero positivo.");
    }
    if (usuarios_.count(usuario.getId()) > 0) {
        throw std::invalid_argument("Ja existe um usuario com esse ID.");
    }

    validaLogin(usuario);
    validaSenha(usuario);

    usuarios_.emplace(usuario.getId(), usuario);
    try {
        salvarUsuarios();
    } catch (...) {
        usuarios_.erase(usuario.getId());
        throw;
    }
}

bool ControleCriarConta::excluirConta(int id) {
    const auto usuario = usuarios_.find(id);
    if (usuario == usuarios_.end()) {
        return false;
    }

    const Usuario removido = usuario->second;
    usuarios_.erase(usuario);
    try {
        salvarUsuarios();
    } catch (...) {
        usuarios_.emplace(id, removido);
        throw;
    }
    return true;
}

const std::map<int, Usuario>& ControleCriarConta::listar() const {
    return usuarios_;
}
