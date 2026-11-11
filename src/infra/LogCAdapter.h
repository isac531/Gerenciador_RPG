#pragma once

#include <cstdio>
#include <string>

#include "../business/log/ILogger.h"

// Padrao Adapter (ADR-0006), versao "adaptador de objeto".
//
// - Target:  ILogger (business), interface orientada a objetos com
//            info/aviso/erro recebendo std::string.
// - Adaptee: biblioteca externa rxi/log.c (infra/externo/logc), uma API C
//            com funcoes globais, niveis inteiros (LOG_INFO, LOG_WARN...),
//            formato estilo printf e destinos registrados por callback.
// - Adapter: esta classe. Converte cada chamada de ILogger em log_log(),
//            passando a mensagem como argumento de "%s" (um login digitado
//            com "%" nao vira formato), e liga a biblioteca a um arquivo.
//
// A biblioteca guarda os destinos em variaveis globais; por isso so um
// LogCAdapter fica ativo por vez (o ultimo criado). Crie um so, em main.cpp.
class LogCAdapter : public ILogger {
public:
    // Abre (ou cria) `caminho` para acrescentar linhas. Se nao conseguir
    // abrir, o log fica desligado e o programa segue (o log nao e essencial).
    explicit LogCAdapter(const std::string& caminho = "rpg.log");
    ~LogCAdapter() override;

    LogCAdapter(const LogCAdapter&) = delete;
    LogCAdapter& operator=(const LogCAdapter&) = delete;

    void info(const std::string& mensagem) override;
    void aviso(const std::string& mensagem) override;
    void erro(const std::string& mensagem) override;

    // false se o arquivo de log nao pode ser aberto.
    bool ativo() const;

private:
    void registrar(int nivel, const std::string& mensagem) const;

    std::FILE* arquivo_ = nullptr;
};
