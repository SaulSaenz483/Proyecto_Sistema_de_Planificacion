//
// Created by Daniel Castillo on 16/04/2026.
//

#include "EquipoEstandar.h"



void EquipoEstandar::degradar()
{
    double desgaste = 1.0 * criticidad; //Tiene mayor aguante que equipos criticos
    estado -= desgaste;

    if (estado <0)
    {
        estado = 0;
    }

}