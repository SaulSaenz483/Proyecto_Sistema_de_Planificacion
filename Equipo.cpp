//
// Created by Daniel Castillo on 11/04/2026.
//

#include "Equipo.h"
#include "Incidencia.h"

Equipo::Equipo(string id, int criticidad, double estado) : id(id), criticidad(criticidad), estado(estado), tiempoInactivo(0){}

void Equipo::agregarIncidencia(Incidencia* inc)
{
    incidencias.push_back(inc); // Consultar uso de push_back
}

int Equipo::getCantidadIncidencias()
{
    int total = 0;
    for (auto inc : incidencias)
    {
        total += inc -> gravedadIncidencia(); // Polimorfismo
    }
    return total;
}

string Equipo:: getID() const {return id; }
int Equipo:: getCriticidad() const {return criticidad; }
double Equipo:: getEstado() const { return estado;}
int Equipo:: getTiempoInactivo() const { return tiempoInactivo; }

void Equipo:: degradar(){}
void Equipo:: aumentarTiempoInactivo(){}
void Equipo:: resetTiempoInactivo(){}
