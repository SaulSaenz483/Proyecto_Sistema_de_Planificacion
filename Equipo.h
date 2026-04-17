//
// Created by Daniel Castillo on 11/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H

#include <string>
#include<iostream>
#include <vector>
using namespace std;

class Incidencia;

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
    virtual void aplicarMantenimiento() = 0; //Necesitamos que equipo sea abstracta para usar el patron de diseño
    void aumentarTiempoInactivo();
    void resetTiempoInactivo();

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H