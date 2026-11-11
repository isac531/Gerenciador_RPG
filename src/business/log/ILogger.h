#pragma once

#include <string>

// Interface de log que a camada business espera (papel de Target no padrao
// Adapter, ADR-0006). A business registra eventos sem saber qual biblioteca
// grava o log nem onde ele fica; o adaptador concreto mora em src/infra.
//
// Regra: nunca registrar a senha (NF-SEG-01).
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void info(const std::string& mensagem) = 0;
    virtual void aviso(const std::string& mensagem) = 0;
    virtual void erro(const std::string& mensagem) = 0;
};

// Logger que descarta tudo (Null Object). E o padrao da fachada enquanto
// nenhum logger for definido, e evita testar "logger != nullptr" no codigo.
class LoggerNulo : public ILogger {
public:
    void info(const std::string&) override {}
    void aviso(const std::string&) override {}
    void erro(const std::string&) override {}
};
