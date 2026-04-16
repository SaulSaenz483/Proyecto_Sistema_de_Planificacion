//
// Created by N00183949 on 16/4/2026.
//

#include "IncidenciaMedia.h"

IncidenciaMedia::IncidenciaMedia(int dia) : Incidencia(dia){}

int IncidenciaMedia::relevancia()
{
    return 2;
}

string IncidenciaMedia::getTipo()
{
    return "Media";
}
