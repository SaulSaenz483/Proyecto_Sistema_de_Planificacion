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

protected:
    string id;
    int criticidad;
    double estado;
    int tiempoInactivo;

    vector<Incidencia*> incidencias;
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
    virtual void aplicarMantenimiento() = 0; //Necesitamos que equipo sea abstracta para usar el patron de diseño
    double calcularPrioridad()const;
    void aumentarTiempoInactivo();
    void resetTiempoInactivo();

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EQUIPO_H