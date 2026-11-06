#pragma once

#include <optional>

#include "../business/control/FacadeSingletonController.h"

// Submenu de console do CRUD de personagens (UC07, UC12, UC13, UC14).
class MenuPersonagens {
public:
    explicit MenuPersonagens(FacadeSingletonController& fachada);

    // Laco do submenu; retorna false apenas quando a entrada padrao terminou.
    bool executar();

private:
    // Retornam false apenas quando a entrada padrao foi encerrada.
    bool criar();
    bool editar();
    bool excluir();
    void listar() const;

    // Le nome, nivel, pontos de vida e atributos. std::nullopt se algum
    // numero for invalido ou a entrada terminar.
    static std::optional<Ficha> lerFicha();

    FacadeSingletonController& fachada_;
};
