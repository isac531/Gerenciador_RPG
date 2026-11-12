#include "RegistroAcesso.h"

#include <utility>

RegistroAcesso::RegistroAcesso(std::string login, int idUsuario, std::int64_t dataHora,
                               bool sucesso)
    : RegistroAcesso(SEM_ID, std::move(login), idUsuario, dataHora, sucesso) {}

RegistroAcesso::RegistroAcesso(int id, std::string login, int idUsuario,
                               std::int64_t dataHora, bool sucesso)
    : id_(id),
      login_(std::move(login)),
      idUsuario_(idUsuario),
      dataHora_(dataHora),
      sucesso_(sucesso) {}

RegistroAcesso RegistroAcesso::comId(int id) const {
    return RegistroAcesso(id, login_, idUsuario_, dataHora_, sucesso_);
}

int RegistroAcesso::getId() const { return id_; }
const std::string& RegistroAcesso::getLogin() const { return login_; }
int RegistroAcesso::getIdUsuario() const { return idUsuario_; }
std::int64_t RegistroAcesso::getDataHora() const { return dataHora_; }
bool RegistroAcesso::foiSucesso() const { return sucesso_; }
