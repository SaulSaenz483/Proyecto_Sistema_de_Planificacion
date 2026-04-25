//
// Created by Daniel Castillo on 23/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H


#include<vector>
#include<string>
#include <fstream>
#include <sstream> //Para el manejo de la lectura con ;
#include"Equipo.h"
#include "Incidencia.h"

class Sistema
{
private:
    vector<Equipo*> inventario;
    vector<Incidencia*> historialIncidencias;
    int diaActual;

    // Metodo para leer las lineas del txt

    Equipo* fabricarEquipo(const string& linea);
    Incidencia* fabricarIncidencia(const string& linea);

    //Algoritmos propios obligatorios segun el pdf

    void quickSortEquipos(int low, int high);
    int particionEquipos(int low, int high);
    Equipo* busquedaBinariaEquipo(const string& id);

    // Calculos globales
    double calcularRiesgoGlobal()const;
    int calcularBacklogPendiente()const;

public:
    Sistema();
    ~Sistema();

    //RF1 - Cargar datos

    void cargarDatosIniciales(const string& rutaArchivo); //Leera el archivo linea por linea

    //Logica de la simulacion
    void ejecutarSimulacion();
    void procesarDia();

    //RF9 Y RF10: Reportes y persistencia

    void generarReporteDiario();


};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H