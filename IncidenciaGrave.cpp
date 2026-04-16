//
// Created by N00183949 on 16/4/2026.
//

#include "IncidenciaGrave.h"

IncidenciaGrave::IncidenciaGrave(int dia) : Incidencia(dia){}

int IncidenciaGrave::relevancia()
{
    return 3;
}
string IncidenciaGrave::getTipo()
{
    return "Grave";
}
