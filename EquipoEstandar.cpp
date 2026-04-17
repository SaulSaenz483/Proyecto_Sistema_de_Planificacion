//
// Created by Daniel Castillo on 16/04/2026.
//

#include "EquipoEstandar.h"

EquipoEstandar::EquipoEstandar(string id, int criticidad, double estado):Equipo(id,criticidad,estado) {

}

void EquipoEstandar::aplicarMantenimiento() {
    cout <<" Realizando mantenimiento al equipo estandar: " << getID() << endl;
}