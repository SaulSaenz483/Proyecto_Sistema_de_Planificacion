//
// Created by Daniel Castillo on 23/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H

#include <vector>
#include <string>
#include "Equipo.h"
#include "Incidencia.h"
#include <random>

using namespace std;

class Sistema
{
private:
    vector<Equipo*>     inventario;
    vector<Incidencia*> historialIncidencias;
    int                 diaActual;

    // Limite total de incidencias en toda la simulacion
    static constexpr int MAX_INCIDENCIAS = 300;

    // Generador de numeros aleatorios (semilla distinta en cada ejecucion)
    mt19937 rng;

    // Fabricas de objetos — static porque no usan atributos de la clase
    static Equipo*     fabricarEquipo(const string& linea);
    static Incidencia* fabricarIncidencia(const string& linea, string& idEquipoOut);

    // Algoritmos propios obligatorios
    void    quickSortEquipos(int low, int high);
    int     particionEquipos(int low, int high);
    Equipo* busquedaBinariaEquipo(const string& id) const;

    // Calculos globales
    double calcularRiesgoGlobal()    const;
    int    calcularBacklogPendiente() const;

    // Pasos internos de cada dia
    void degradarTodos()const;
    void generarIncidenciasAleatorias();
    void activarIncidenciasDia()const;
    void seleccionarYMantener(vector<Equipo*>& atendidos) const;
    void actualizarTiemposInactivos(const vector<Equipo*>& atendidos) const;

public:
    Sistema();
    ~Sistema();

    // RF1: Carga de datos desde archivo
    void cargarDatosIniciales(const string& rutaArchivo);

    // Logica de la simulacion
    void ejecutarSimulacion();
    void procesarDia();

    // RF9 + RF10: Reportes y persistencia
    void generarReporteDiario(const vector<Equipo*>& atendidos) const;
};

#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_SISTEMA_H