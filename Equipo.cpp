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

double Equipo::calcularPrioridad()
{

    double prioridad = (criticidad * 0.5) +
                       (getCantidadIncidencias() * 0.3) +
                       (tiempoInactivo * 0.2);
    return prioridad;
}

string Equipo:: getID() const {return id; }
int Equipo:: getCriticidad() const {return criticidad; }
double Equipo:: getEstado() const { return estado;}
int Equipo:: getTiempoInactivo() const { return tiempoInactivo; }


void Equipo:: aumentarTiempoInactivo(){tiempoInactivo++;} //Le suma 1 al contador
void Equipo:: resetTiempoInactivo(){tiempoInactivo=0;} //Lo devuelve a 0 despues de un mantenimiento
