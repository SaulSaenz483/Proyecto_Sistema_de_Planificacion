//
// Created by Daniel Castillo on 16/04/2026.
//

#include "EquipoCritico.h"

EquipoCritico::EquipoCritico(string id, int criticidad, double estado): Equipo(id,criticidad,estado) {


}

void EquipoCritico:: aplicarMantenimiento() {
    cout <<" Realizando mantenimiento al equipo critico: " << getID() << endl;
}





