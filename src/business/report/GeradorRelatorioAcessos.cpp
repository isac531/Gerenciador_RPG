#include "GeradorRelatorioAcessos.h"

#include <cstdio>
#include <ctime>

namespace {

const std::string kTitulo = "Relatorio de acessos";

std::string numero(std::size_t valor) { return std::to_string(valor); }

std::string percentual(double valor) {
    char texto[16];
    std::snprintf(texto, sizeof(texto), "%.1f%%", valor);
    return texto;
}

std::string dataOuTraco(const std::optional<std::int64_t>& instante) {
    return instante ? GeradorRelatorioAcessos::formatarDataHora(*instante) : "-";
}

}  // namespace

std::string GeradorRelatorioAcessos::gerar(const EstatisticasAcesso& estatisticas) {
    iniciarDocumento(kTitulo + " - Gerenciador de RPG");
    escreverTitulo(kTitulo);
    escreverParagrafo("Gerado em " + formatarDataHora(estatisticas.geradoEm) +
                      " (horario local).");
    escreverResumo(estatisticas);
    escreverAcessosPorUsuario(estatisticas);
    escreverParagrafo(textoRodape());
    return finalizarDocumento();
}

void GeradorRelatorioAcessos::escreverResumo(const EstatisticasAcesso& e) {
    const auto maisAtivo = e.usuarioMaisAtivo();
    escreverSubtitulo("Resumo");
    escreverTabela(
        {"Indicador", "Valor"},
        {
            {"Tentativas de login", numero(e.totalTentativas)},
            {"Acessos aceitos", numero(e.acessosAceitos)},
            {"Tentativas recusadas", numero(e.tentativasRecusadas)},
            {"Recusas com login inexistente", numero(e.tentativasLoginInexistente)},
            {"Taxa de sucesso", percentual(e.taxaDeSucesso())},
            {"Usuarios cadastrados", numero(e.usuariosCadastrados)},
            {"Usuarios que ja acessaram", numero(e.usuariosQueAcessaram)},
            {"Usuario mais ativo",
             maisAtivo ? maisAtivo->login + " (" + numero(maisAtivo->acessos) + ")" : "-"},
            {"Primeira tentativa", dataOuTraco(e.primeiraTentativa)},
            {"Ultima tentativa", dataOuTraco(e.ultimaTentativa)},
        });
}

void GeradorRelatorioAcessos::escreverAcessosPorUsuario(const EstatisticasAcesso& e) {
    escreverSubtitulo("Acessos por usuario");
    if (e.porUsuario.empty()) {
        escreverParagrafo("Nenhum usuario cadastrado.");
        return;
    }
    std::vector<Linha> linhas;
    for (const EstatisticaUsuario& usuario : e.porUsuario) {
        linhas.push_back({std::to_string(usuario.idUsuario), usuario.login,
                          papelParaTexto(usuario.papel), numero(usuario.acessos),
                          numero(usuario.recusas), dataOuTraco(usuario.ultimoAcesso)});
    }
    escreverTabela({"ID", "Login", "Papel", "Acessos", "Recusas", "Ultimo acesso"}, linhas);
}

std::string GeradorRelatorioAcessos::textoRodape() const {
    return "Gerenciador de RPG - estatisticas calculadas a partir do historico de logins.";
}

std::string GeradorRelatorioAcessos::formatarDataHora(std::int64_t segundosDesde1970) {
    const std::time_t instante = static_cast<std::time_t>(segundosDesde1970);
    const std::tm* local = std::localtime(&instante);
    if (local == nullptr) {
        return "-";
    }
    char texto[32];
    std::strftime(texto, sizeof(texto), "%d/%m/%Y %H:%M", local);
    return texto;
}
