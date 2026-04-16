//
// Created by Daniel Castillo on 11/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H

#include <string>
#include <vector>
using namespace std;

class Incidencia; // Declaracion adelantada

class Equipo {

private:
    string id;
    int criticidad;
    double estado;
    int tiempoInactivo;

    vector<Incidencia*> incidencias;
public:

    Equipo(string id, int criticidad, double estado);

    // Gestion de incidencias
    void agregarIncidencia(Incidencia* inc);
    int getCantidadIncidencias();

    string getID()const ;
    int getCriticidad()const ;
    double getEstado()const ;
    int getTiempoInactivo()const ;

    void degradar();
    void aplicarMantenimiento();
    void aumentarTiempoInactivo();
    void resetTiempoInactivo();

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H