//
// Created by N00183949 on 16/4/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H

#include <string>
using namespace std;
class Equipo; // Declaracion adelantada

enum class TipoIncidencia
{
    LEVE, MEDIA, GRAVE
};

class Incidencia {

private:
    TipoIncidencia tipo;
    string descripcion;
    int diaRegistro;
    Equipo* equipoAsociado;

public:
    Incidencia(TipoIncidencia tipo, string descripcion, int diaRegistro);

    // Retorna el tipo enum
    TipoIncidencia getTipo() const;

    string getTipoString() const;

    int getPeso()const;

    string getDescripcion()const;
    int getDiaRegistro()const;

    // Conexion con Equipo

    void asignarEquipo(Equipo* equipo);
    Equipo* getEquipo()const;


};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H