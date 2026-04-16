//
// Created by N00183949 on 16/4/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H


#include <string>
using namespace std;

class Incidencia {
protected:
    int dia;
public:
    Incidencia(int dia);
    virtual ~Incidencia ();
    virtual int relevancia() = 0;
    virtual string getTipo() = 0;

};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_INCIDENCIA_H