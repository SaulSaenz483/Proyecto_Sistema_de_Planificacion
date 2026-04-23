//
// Created by N00183949 on 22/4/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_EXCEPCIONES_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_EXCEPCIONES_H

#include <exception>
#include <string>
using namespace std;

// Clase Base

class SistemaException : public exception
{
protected:
    string mensaje;

public:
    explicit SistemaException(const string &mensaje) : mensaje(mensaje) {}

    const char* what() const noexcept override { return mensaje.c_str(); }
};

// Derivadas

class ArchivoInvalidoException : public SistemaException
{
public:
    explicit ArchivoInvalidoException(const string& mensaje) : SistemaException("Archivo Invalido: " + mensaje) {}
};

class FormatoInvalidoException : public SistemaException
{
public:
    explicit FormatoInvalidoException(const string& mensaje) : SistemaException("Formato Invalido: " + mensaje) {}
};

class OperacionInconsistenteException : public SistemaException
{
public:
    explicit OperacionInconsistenteException(const string& mensaje) : SistemaException("Operacion inconsistente: " + mensaje) {}
};

#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_EXCEPCIONES_H