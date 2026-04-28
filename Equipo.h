//
// Created by Daniel Castillo on 11/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H

#include <string>
#include<iostream>
#include <vector>
#include "Estrategias.h"
using namespace std;

class Incidencia;

class Equipo {

protected:
    string id;
    int criticidad;
    double estado;
    int tiempoInactivo;

    vector<Incidencia*> incidencias;
    EstrategiaMantenimiento* estrategiaActual;

public:

    Equipo(string id, int criticidad, double estado);
    virtual ~Equipo();

    // Gestion de incidencias
    void agregarIncidencia(Incidencia* inc);
    void limpiarIncidencias();
    int getCantidadIncidencias()const;
    int getPesoTotalIncidencias()const;

    string getID()const ;
    int getCriticidad()const ;
    double getEstado()const ;
    int getTiempoInactivo()const ;

    virtual void degradar()=0;
    virtual void aplicarMantenimiento();
    double calcularPrioridad()const;
    void aumentarTiempoInactivo();
    void resetTiempoInactivo();
    void setEstrategia(EstrategiaMantenimiento* nuevaEstrategia);
    void setEstado(double nuevoEstado);

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H