#include "LogCAdapter.h"

#include <ctime>

// A biblioteca e C e o cabecalho nao declara extern "C": sem isto o
// compilador C++ procuraria nomes "decorados" que nao existem em log.o.
extern "C" {
#include "externo/logc/log.h"
}

namespace {

// Destino atual. A log.c chama callbacks globais; o callback abaixo e
// registrado uma unica vez e escreve no arquivo do adaptador ativo.
std::FILE* destinoAtual = nullptr;
bool callbackRegistrado = false;

// Formato de cada linha: "2026-11-11 21:30:00 INFO  mensagem".
void escreverNoDestino(log_Event* evento) {
    if (destinoAtual == nullptr) {
        return;
    }
    char dataHora[32];
    dataHora[std::strftime(dataHora, sizeof(dataHora), "%Y-%m-%d %H:%M:%S", evento->time)] =
        '\0';
    std::fprintf(destinoAtual, "%s %-5s ", dataHora, log_level_string(evento->level));
    std::vfprintf(destinoAtual, evento->fmt, evento->ap);
    std::fputc('\n', destinoAtual);
    std::fflush(destinoAtual);
}

}  // namespace

LogCAdapter::LogCAdapter(const std::string& caminho)
    : arquivo_(std::fopen(caminho.c_str(), "a")) {
    // Por padrao a log.c tambem escreve em stderr, o que poluiria o menu.
    log_set_quiet(true);
    if (!callbackRegistrado) {
        log_add_callback(escreverNoDestino, nullptr, LOG_INFO);
        callbackRegistrado = true;
    }
    destinoAtual = arquivo_;
}

LogCAdapter::~LogCAdapter() {
    if (destinoAtual == arquivo_) {
        destinoAtual = nullptr;
    }
    if (arquivo_ != nullptr) {
        std::fclose(arquivo_);
    }
}

void LogCAdapter::registrar(int nivel, const std::string& mensagem) const {
    if (arquivo_ == nullptr) {
        return;
    }
    // Arquivo e linha nao sao gravados: o formato de linha e decidido aqui.
    log_log(nivel, __FILE__, __LINE__, "%s", mensagem.c_str());
}

void LogCAdapter::info(const std::string& mensagem) { registrar(LOG_INFO, mensagem); }
void LogCAdapter::aviso(const std::string& mensagem) { registrar(LOG_WARN, mensagem); }
void LogCAdapter::erro(const std::string& mensagem) { registrar(LOG_ERROR, mensagem); }

bool LogCAdapter::ativo() const { return arquivo_ != nullptr; }
