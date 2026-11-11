#include "ControleRelatorioAcessos.h"

#include <map>
#include <utility>

ControleRelatorioAcessos::ControleRelatorioAcessos(const IUsuarioRepositorio& usuarios,
                                                   const IAcessoRepositorio& acessos,
                                                   ControleLogin::Relogio relogio)
    : usuarios_(usuarios), acessos_(acessos), relogio_(std::move(relogio)) {}

EstatisticasAcesso ControleRelatorioAcessos::calcularEstatisticas() const {
    EstatisticasAcesso estatisticas;
    estatisticas.geradoEm = relogio_();

    std::map<int, EstatisticaUsuario> porId;
    for (const Usuario& usuario : usuarios_.listarTodos()) {
        EstatisticaUsuario item;
        item.idUsuario = usuario.getId();
        item.login = usuario.getLogin();
        item.papel = usuario.getPapel();
        porId.emplace(usuario.getId(), item);
    }

    for (const RegistroAcesso& registro : acessos_.listarTodos()) {
        const std::int64_t instante = registro.getDataHora();
        ++estatisticas.totalTentativas;
        if (!estatisticas.primeiraTentativa || instante < *estatisticas.primeiraTentativa) {
            estatisticas.primeiraTentativa = instante;
        }
        if (!estatisticas.ultimaTentativa || instante > *estatisticas.ultimaTentativa) {
            estatisticas.ultimaTentativa = instante;
        }

        if (registro.foiSucesso()) {
            ++estatisticas.acessosAceitos;
        } else {
            ++estatisticas.tentativasRecusadas;
            if (registro.getIdUsuario() == SEM_ID) {
                ++estatisticas.tentativasLoginInexistente;
            }
        }

        // Registros de usuarios ja excluidos entram so nos totais.
        const auto usuario = porId.find(registro.getIdUsuario());
        if (usuario == porId.end()) {
            continue;
        }
        EstatisticaUsuario& item = usuario->second;
        if (registro.foiSucesso()) {
            ++item.acessos;
            if (!item.ultimoAcesso || instante > *item.ultimoAcesso) {
                item.ultimoAcesso = instante;
            }
        } else {
            ++item.recusas;
        }
    }

    estatisticas.usuariosCadastrados = porId.size();
    for (auto& entrada : porId) {
        if (entrada.second.acessos > 0) {
            ++estatisticas.usuariosQueAcessaram;
        }
        estatisticas.porUsuario.push_back(std::move(entrada.second));
    }
    return estatisticas;
}

RelatorioGerado ControleRelatorioAcessos::gerar(FormatoRelatorio formato) const {
    const auto gerador = criarGeradorRelatorio(formato);
    return {gerador->gerar(calcularEstatisticas()), gerador->extensao()};
}
