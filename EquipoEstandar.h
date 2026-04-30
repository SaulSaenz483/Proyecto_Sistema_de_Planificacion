//
// Created by Daniel Castillo on 16/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPOESTANDAR_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPOESTANDAR_H

#include "Equipo.h"

class EquipoEstandar:public Equipo {

public:

    EquipoEstandar(const string& id, int criticidad, double estado):Equipo(id,criticidad,estado){}

    void degradar() override;



};




#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPOESTANDAR_H