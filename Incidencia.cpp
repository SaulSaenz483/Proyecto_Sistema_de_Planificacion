//
// Created by N00183949 on 16/4/2026.
//

#include "Incidencia.h"
#include "Equipo.h"
#include "Excepciones.h"

Incidencia::Incidencia(TipoIncidencia tipo, string descripcion, int diaRegistro) : tipo(tipo),
descripcion(descripcion),diaRegistro(diaRegistro), equipoAsociado(nullptr)
{
    if (descripcion.empty())
    {
        throw FormatoInvalidoException("Las descripcion de la incidencia no puede estar vacia");
    }
    if (diaRegistro < 1 || diaRegistro > 30)
    {
        throw FormatoInvalidoException("El dia del registro debe estar entre 1 y 30");
    }
}

TipoIncidencia Incidencia::getTipo()const
{
    return tipo;
}

string Incidencia::getTipoString() const
{
    switch (tipo)
    {
    case TipoIncidencia::LEVE : return "LEVE";
    case TipoIncidencia::MEDIA : return "MEDIA";
    case TipoIncidencia::GRAVE : return "GRAVE";
    default: throw FormatoInvalidoException("El tipo de incidencia es incorrecto.");;
    }
}

int Incidencia::getPeso() const
{
    switch (tipo)
    {
        case TipoIncidencia::LEVE : return 1;
        case TipoIncidencia::MEDIA : return 2;
        case TipoIncidencia::GRAVE : return 3;
        default : throw FormatoInvalidoException("El peso de incidencia es incorrecto.");
    }
}

string Incidencia::getDescripcion() const
{
    return descripcion;
}

int Incidencia::getDiaRegistro() const
{
    return diaRegistro;
}

void Incidencia::asignarEquipo(Equipo* equipo)
{
    if (equipo == nullptr)
    {
        throw OperacionInconsistenteException("No se le puede asignar un equipo nulo a una incidencia");
    }
    equipoAsociado = equipo;
}

Equipo* Incidencia::getEquipo() const
{
    return equipoAsociado;
}
