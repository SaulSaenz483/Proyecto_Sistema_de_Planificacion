//
// Created by N00183949 on 16/4/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIAGRAVE_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIAGRAVE_H
#include "Incidencia.h"


class IncidenciaGrave : public Incidencia
{
public:
    IncidenciaGrave(int dia);
    int relevancia() override;
    string getTipo() override;
};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIAGRAVE_H