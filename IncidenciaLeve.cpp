//
// Created by N00183949 on 16/4/2026.
//

#include "IncidenciaLeve.h"

IncidenciaLeve::IncidenciaLeve(int dia) : Incidencia(dia) {}

int IncidenciaLeve::relevancia()
{
    return 1;
}

string IncidenciaLeve::getTipo()
{
    return "Leve";
}




