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

Incidencia* fabricarIncidencia(const string& linea)
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
