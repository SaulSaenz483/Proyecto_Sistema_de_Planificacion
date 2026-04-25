//
// Created by Daniel Castillo on 23/04/2026.
//

#include "Sistema.h"
#include "EquipoCritico.h" //Para poder crear los objetos
#include "EquipoEstandar.h"
#include "Excepciones.h"
#include<sstream> //Para cortar los strings
#include <iostream>

using namespace std;

Sistema::Sistema(): diaActual(1){}

Sistema::~Sistema(){}

Equipo* Sistema::fabricarEquipo(const string& linea)
{
    stringstream ss(linea);
    string id, tokenCriticidad, tokenEstado;

    getline(ss,id,';'); // Lee hasta que encuentra ; y guarda lo que leyo en una variable
    getline(ss,tokenCriticidad,';');
    getline(ss,tokenEstado,';');

    //Creamos variables limpias para guardar los datos

    int criticidad =0;
    double estado = 0.0;

    size_t posCriticidad = tokenCriticidad.find('='); // Limpiar "criticidad=9" para sacar solo el '9'
    if (posCriticidad != string::npos) //Si encuentra el signo '='
    {
        criticidad = stoi(tokenCriticidad.substr(posCriticidad + 1)); //Da el pedazo de texto que esta despues del igual
    }

    size_t posEstado = tokenEstado.find('=');
    if (posEstado != string::npos)
    {
        estado = stod(tokenEstado.substr(posEstado + 1));
    }

    //La decision de crear equipo basado en su criticidad
    if (criticidad >=8)
    {
        return new EquipoCritico(id,criticidad,estado);
    } else
    {
        return new EquipoEstandar(id,criticidad,estado);
    }

}

Incidencia* Sistema::fabricarIncidencia(const string& linea)
{
    stringstream ss(linea);
    string prefijo, idEquipo, tokenSeveridad, tokenDia;

    // Ejemplo: INC;EQ-001;severidad=ALTA;dia=3

    getline(ss, prefijo,';');
    getline(ss, idEquipo,';');
    getline(ss, tokenSeveridad,';');
    getline(ss, tokenDia,';');

    string severidadStr = ""; //Extraemos el valor de la severidad despues del '='
    size_t posSeveridad = tokenSeveridad.find('=');

    if (posSeveridad!=string::npos)
    {
        severidadStr = tokenSeveridad.substr(posSeveridad+1);
    }

    // Traducir el texto al ENUM

    TipoIncidencia tipo = TipoIncidencia::LEVE; //Valor por defecto

    if (severidadStr == "ALTA" || severidadStr == "GRAVE") {
        tipo = TipoIncidencia::GRAVE;

    } else if (severidadStr == "MEDIA") {
        tipo = TipoIncidencia::MEDIA;

    } else if (severidadStr == "BAJA"||severidadStr == "LEVE" ) {
        tipo = TipoIncidencia::LEVE;
    }

    //Extraer el valor del dia

    int dia = 1; //Creamos una variable limpia
    size_t posDia = tokenDia.find('=');

    if (posDia!= string::npos) //Busca hasta '='
    {
        dia = stoi(tokenDia.substr(posDia+1)); //Extrae lo que hay despues del '=' y convierte dia a entero
    }

    //Creamos una descripcion, ya que el txt que nos daban no traia descripcion

    string descripcion = "Incidencia reportada para el equipo " + idEquipo;

    //Fabricamos la incidencia

    Incidencia* nuevaInc = new Incidencia(tipo,descripcion,dia);

    return nuevaInc;

}


int Sistema::particionEquipos(int low, int high)
{
    //Tomar prioridad del ultimo equipo como pivote
    double pivotPrioridad = inventario[high]->calcularPrioridad();

    //Indice del elemento mas grande

    int i = (low -1);

    for (int j = low; j <= high-1; j++)
    {
        if (inventario[j]->calcularPrioridad() >= pivotPrioridad) //Vamos de forma descendente
        {
            i++; //Avanzar el indice

            //Hacemos un intercambio de punteros en el vector

            Equipo* temp = inventario[i];
            inventario[i] = inventario[j];
            inventario[j] = temp;

        }
    }

    //Colocamos el pivote en su lugar

    Equipo* temp = inventario[i+1];
    inventario[i+1] = inventario[high];
    inventario[high] = temp;

    return (i+1); //Retornamos la posicion final del pivote

}

void Sistema::quickSortEquipos(int low, int high)
{

    if (low < high)
    {

        // pi es el índice donde quedó el pivote, en este caso ya estaria en su posicion final

        int pi = particionEquipos(low, high);

        //Ordenamos recursivamente la mitad izquierda
        quickSortEquipos(low, pi-1);

        //Ordenamos recursivamente la mitad derecha
        quickSortEquipos(pi+1, high);

    }

}

Equipo* Sistema::busquedaBinariaEquipo(const string& idBusqueda)
{
    int izquierda = 0;
    int derecha = inventario.size() - 1;

    while (izquierda<= derecha)
    {
        int medio = izquierda + (derecha - izquierda)/2;
        string idActual = inventario[medio]->getID();

        if (idActual == idBusqueda)
        {
            return inventario[medio];
        }

        if (idActual < idBusqueda)
        {
            izquierda = medio + 1; //Si idActual es mayor alfanumericamente, ignora la izquierda)
        } else {
            derecha = medio - 1; //Si es menor, ignora la derecha
        }

    }
    return nullptr; //Regresa nulo si el equipo no existe
}

double Sistema::calcularRiesgoGlobal() const
{
    double riesgoTotal = 0.0;

    //Recorremos el historial completo de incidencias

    for (Incidencia* inc : historialIncidencias)
    {
        riesgoTotal+= inc->getPeso();
    }
    return riesgoTotal;
}


int Sistema::calcularBacklogPendiente() const {
    int totalIncidencias;

    for (Equipo* eq: inventario)
    {
        totalIncidencias += eq->getCantidadIncidencias();
    }

    return totalIncidencias;

}



