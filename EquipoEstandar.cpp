//
// Created by Daniel Castillo on 16/04/2026.
//

#include "EquipoEstandar.h"



void EquipoEstandar::degradar()
{
    double desgaste = 1.0 * criticidad;v //Tiene mayor aguante que equipos criticos
    estado = estado - desgaste;

    if (estado <0)
    {
        estado = 0;
    }

}

void EquipoEstandar::aplicarMantenimiento()  {
    cout <<" Realizando mantenimiento al equipo estandar: " << getID() << endl;

    estado = 100.0;

    resetTiempoInactivo();


}