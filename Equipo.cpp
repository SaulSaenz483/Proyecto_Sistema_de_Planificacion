//
// Created by Daniel Castillo on 11/04/2026.
//

#include "Equipo.h"
#include "Incidencia.h"
#include "Excepciones.h"

Equipo::Equipo(string id, int criticidad, double estado) : id(id), criticidad(criticidad), estado(estado), tiempoInactivo(0),estrategiaActual(nullptr)
{
    if (id.empty()) {
        throw FormatoInvalidoException("El ID del equipo no puede estar vacio.");
    }
    if (criticidad < 1 || criticidad > 10) {
        throw FormatoInvalidoException("La criticidad debe estar entre 1 y 10.");
    }
    if (estado < 0.0 || estado > 100.0) {
        throw FormatoInvalidoException("El estado debe estar entre 0 y 100.");
    }
}

Equipo::~Equipo()
{
    // El equipo no es dueño de las incidencias (solo las referencia),
    // asi que no las eliminamos aqui. El sistema las maneja por separado.
}

void Equipo::agregarIncidencia(Incidencia* inc)
{
    if (inc == nullptr) {
        throw OperacionInconsistenteException("No se puede agregar una incidencia nula al equipo " + id);
    }
    incidencias.push_back(inc);}

int Equipo::getCantidadIncidencias()const
{
    return static_cast<int>(incidencias.size()); //Retorna el tam del vector (las incidencias activas)
}

int Equipo::getPesoTotalIncidencias()const
{
    int total = 0;

    for (auto inc : incidencias)
    {
        total += inc -> getPeso();
    }
    return total;
}

string Equipo:: getID() const {return id; }
int Equipo:: getCriticidad() const {return criticidad; }
double Equipo:: getEstado() const { return estado;}
int Equipo:: getTiempoInactivo() const { return tiempoInactivo; }

void Equipo::limpiarIncidencias() {
    incidencias.clear();
}

double Equipo::calcularPrioridad() const
{
    double prioridad = (criticidad * 0.5)+ (getPesoTotalIncidencias() * 0.3)+ (tiempoInactivo * 0.2);

    return prioridad;
}


void Equipo:: aumentarTiempoInactivo()
{
    tiempoInactivo++;
}
void Equipo:: resetTiempoInactivo()
{
    tiempoInactivo = 0;
}

void Equipo::setEstrategia(EstrategiaMantenimiento* nuevaEstrategia)
{
    estrategiaActual = nuevaEstrategia;
}

void Equipo::aplicarMantenimiento()
{
    if (estrategiaActual != nullptr) {
        estrategiaActual->ejecutarMantenimiento(this);
    } else {
        // Por si acaso se nos olvida asignarle una, hacemos un mantenimiento base
        this->estado += 10;
        this->limpiarIncidencias();
    }
}

void Equipo::setEstado(double nuevoEstado)
{
    this->estado = nuevoEstado;
}
