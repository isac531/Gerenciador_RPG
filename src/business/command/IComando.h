#pragma once

#include <string>

// Interface do padrao Command (ADR-0008): um pedido da ui encapsulado em um
// objeto. O comando guarda os parametros do pedido e conhece os controles
// (Receiver) que o atendem; quem o executa (InvocadorComandos) nao precisa
// saber qual caso de uso esta rodando.
class IComando {
public:
    virtual ~IComando() = default;

    // Executa o caso de uso. As excecoes de negocio (login invalido, ficha
    // invalida, ...) sobem para quem chamou a fachada.
    virtual void executar() = 0;

    // Texto do log quando o armazenamento falha durante o comando,
    // ex.: "Falha ao criar conta". O invocador acrescenta o motivo.
    virtual std::string descricaoFalha() const = 0;
};
