//
// Created by Daniel Castillo on 23/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H


#include<vector>
#include<string>
#include"Equipo.h"
#include "Incidencia.h"
#include <random>

class Sistema
{
private:
    vector<Equipo*> inventario;
    vector<Incidencia*> historialIncidencias;
    int diaActual;
    // Cada dia genera su propio archivo de reporte (no hay stream persistente)

    // Limite de las incidencias
    static const int MAX_INCIDENCIAS = 300;
    // Generador de numeros aleatorios
    mt19937 rng;

    // Metodo para leer las lineas del txt

    Equipo* fabricarEquipo(const string& linea);
    Incidencia* fabricarIncidencia(const string& linea, string& idEquipoOut);

    // Algoritmos propios obligatorios segun el pdf

    void quickSortEquipos(int low, int high);
    int particionEquipos(int low, int high);
    Equipo* busquedaBinariaEquipo(const string& id);

    // Calculos globales
    double calcularRiesgoGlobal()const;
    int calcularBacklogPendiente()const;

    // Procesamiento de dias

    void degradarTodos();
    void generarIncidenciasAleatorias();
    void activarIncidenciasDia(); // Asigna al equipo las incidencias del diaActual
    void seleccionarYMantener(vector<Equipo*>& atendidos); // Top 3 + mantenimientos
    void actualizarTiemposInactivos(const vector<Equipo*>& atendidos);

public:
    Sistema();
    ~Sistema();

    // RF1 - Cargar datos

    void cargarDatosIniciales(const string& rutaArchivo); //Leera el archivo linea por linea

    // Logica de la simulacion
    void ejecutarSimulacion();
    void procesarDia();

    // RF9 Y RF10: Reportes y persistencia

    void generarReporteDiario(const vector<Equipo*>& atendidos);


};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H