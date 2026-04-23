//
// Created by Daniel Castillo on 16/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPOCRITICO_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPOCRITICO_H

#include "Equipo.h"

class EquipoCritico: public Equipo {

public:
    EquipoCritico(string id, int criticidad, double estado): Equipo(id,criticidad,estado){}

    void degradar() override;
    void aplicarMantenimiento() override; //Cumplimos el contrato de equipo

    //Metodo especial especifico para los equipos criticos
    void calibrarMotor();


};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPOCRITICO_H