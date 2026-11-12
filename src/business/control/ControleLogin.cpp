#include "ControleLogin.h"

#include <chrono>
#include <optional>
#include <utility>

#include "../exception/Excecoes.h"

std::int64_t ControleLogin::agora() {
    using namespace std::chrono;
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

ControleLogin::ControleLogin(IUsuarioRepositorio& usuarios, IAcessoRepositorio& acessos,
                             Relogio relogio)
    : usuarios_(usuarios), acessos_(acessos), relogio_(std::move(relogio)) {}

Usuario ControleLogin::autenticar(const std::string& login, const std::string& senha) {
    const std::optional<Usuario> usuario = usuarios_.buscarPorLogin(login);
    const bool sucesso = usuario && usuario->fazerLogin(login, senha);
    const int idUsuario = usuario ? usuario->getId() : SEM_ID;

    acessos_.adicionar(RegistroAcesso(login, idUsuario, relogio_(), sucesso));

    if (!sucesso) {
        throw CredenciaisInvalidasException();
    }
    return *usuario;
}
