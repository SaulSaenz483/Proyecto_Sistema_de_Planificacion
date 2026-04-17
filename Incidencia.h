//
// Created by N00183949 on 16/4/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H


#include <string>

class Equipo; //Declaracion adelantada

using namespace std;

class Incidencia {

    protected:

    int dia;
    Equipo* equipoAsignado; //Creamos la dependencia mutua


public:
    Incidencia(int dia, Equipo* equipoAsignado); //Lo incluimos en el constructor a equipo
   ~Incidencia ();
    int gravedadIncidencia(); //No necesitamos mas clases incidencia, con un atributo y un metodo de su gravedad, podemos ver su nivel de incidencia en los equipos
    string getTipo();

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H