//
// Created by N00183949 on 16/4/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIAMEDIA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIAMEDIA_H
#include "Incidencia.h"


class IncidenciaMedia : public Incidencia
{
public:
    IncidenciaMedia(int dia);
    int relevancia() override;
    string getTipo() override;

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIAMEDIA_H