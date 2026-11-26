#include "ControlePersonagem.h"

#include "../exception/Excecoes.h"
#include "../validation/RegrasFicha.h"

// Para criar uma nova regra da ficha: implemente IRegraFicha e registre aqui.
ControlePersonagem::ControlePersonagem(IPersonagemRepositorio& personagens,
                                       const IUsuarioRepositorio& usuarios)
    : personagens_(personagens), usuarios_(usuarios) {
    regrasFicha_.push_back(std::make_unique<NomePersonagemRegra>());
    regrasFicha_.push_back(std::make_unique<NivelRegra>());
    regrasFicha_.push_back(std::make_unique<PontosDeVidaRegra>());
    regrasFicha_.push_back(std::make_unique<AtributosRegra>());
}

void ControlePersonagem::validarFicha(const Ficha& ficha) const {
    for (const auto& regra : regrasFicha_) {
        regra->validar(ficha);
    }
}

int ControlePersonagem::criar(const Personagem& personagem) {
    if (!usuarios_.existe(personagem.getIdDono())) {
        throw IdInvalidoException("Nao existe usuario com esse ID para ser o dono.");
    }
    validarFicha(personagem.getFicha());
    return personagens_.adicionar(personagem);
}

MementoPersonagem ControlePersonagem::editarFicha(int id, const Ficha& ficha) {
    const std::optional<Personagem> atual = personagens_.buscar(id);
    if (!atual) {
        throw IdInvalidoException("Nao existe personagem com esse ID.");
    }
    validarFicha(ficha);
    personagens_.atualizar(atual->comFicha(ficha));
    return atual->criarMemento();
}

std::optional<Personagem> ControlePersonagem::restaurar(const MementoPersonagem& memento) {
    const std::optional<Personagem> atual = personagens_.buscar(memento.getIdPersonagem());
    if (!atual) {
        return std::nullopt;
    }
    const Personagem restaurado = atual->restaurar(memento);
    personagens_.atualizar(restaurado);
    return restaurado;
}

bool ControlePersonagem::excluir(int id) {
    return personagens_.remover(id);
}

std::size_t ControlePersonagem::excluirDoDono(int idDono) {
    std::size_t excluidos = 0;
    for (const Personagem& personagem : personagens_.listarTodos()) {
        if (personagem.getIdDono() == idDono && personagens_.remover(personagem.getId())) {
            ++excluidos;
        }
    }
    return excluidos;
}

std::optional<Personagem> ControlePersonagem::buscar(int id) const {
    return personagens_.buscar(id);
}

std::vector<Personagem> ControlePersonagem::listar() const {
    return personagens_.listarTodos();
}

std::size_t ControlePersonagem::quantidade() const {
    return personagens_.quantidade();
}
