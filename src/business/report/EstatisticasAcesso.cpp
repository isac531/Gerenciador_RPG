#include "EstatisticasAcesso.h"

double EstatisticasAcesso::taxaDeSucesso() const {
    if (totalTentativas == 0) {
        return 0.0;
    }
    return 100.0 * static_cast<double>(acessosAceitos) / static_cast<double>(totalTentativas);
}

std::optional<EstatisticaUsuario> EstatisticasAcesso::usuarioMaisAtivo() const {
    std::optional<EstatisticaUsuario> maisAtivo;
    for (const EstatisticaUsuario& usuario : porUsuario) {
        if (usuario.acessos > 0 && (!maisAtivo || usuario.acessos > maisAtivo->acessos)) {
            maisAtivo = usuario;
        }
    }
    return maisAtivo;
}
