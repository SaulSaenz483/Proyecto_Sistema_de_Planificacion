//
// Created by Daniel Castillo on 23/04/2026.
//

#include "Sistema.h"
#include <fstream>
#include "EquipoCritico.h"
#include "EquipoEstandar.h"
#include "Excepciones.h"
#include "Estrategias.h"
#include <sstream>
#include <iostream>

using namespace std;

// ================================================================
// Constructor / Destructor
// ================================================================

Sistema::Sistema() : diaActual(1), rng(random_device{}()) {}

Sistema::~Sistema()
{
    for (Equipo*    eq  : inventario)          delete eq;
    for (Incidencia* inc : historialIncidencias) delete inc;
}

// ================================================================
// Fabricas (static — no usan atributos de la clase)
// ================================================================

Equipo* Sistema::fabricarEquipo(const string& linea)
{
    stringstream ss(linea);
    string id, tokenCriticidad, tokenEstado;

    getline(ss, id,              ';');
    getline(ss, tokenCriticidad, ';');
    getline(ss, tokenEstado,     ';');

    int    criticidad = 0;
    double estado     = 0.0;

    size_t posCriticidad = tokenCriticidad.find('=');
    if (posCriticidad != string::npos)
        criticidad = stoi(tokenCriticidad.substr(posCriticidad + 1));

    size_t posEstado = tokenEstado.find('=');
    if (posEstado != string::npos)
        estado = stod(tokenEstado.substr(posEstado + 1));

    if (criticidad >= 8)
        return new EquipoCritico(id, criticidad, estado);
    else
        return new EquipoEstandar(id, criticidad, estado);
}

Incidencia* Sistema::fabricarIncidencia(const string& linea, string& idEquipoOut)
{
    stringstream ss(linea);
    string prefijo, tokenSeveridad, tokenDia;

    // Formato: INC;EQ-001;severidad=ALTA;dia=3
    getline(ss, prefijo,        ';');
    getline(ss, idEquipoOut,    ';');
    getline(ss, tokenSeveridad, ';');
    getline(ss, tokenDia,       ';');

    string severidadStr;
    size_t posSeveridad = tokenSeveridad.find('=');
    if (posSeveridad != string::npos)
        severidadStr = tokenSeveridad.substr(posSeveridad + 1);

    TipoIncidencia tipo = TipoIncidencia::LEVE;
    if      (severidadStr == "ALTA"  || severidadStr == "GRAVE") tipo = TipoIncidencia::GRAVE;
    else if (severidadStr == "MEDIA")                            tipo = TipoIncidencia::MEDIA;
    else if (severidadStr == "BAJA"  || severidadStr == "LEVE")  tipo = TipoIncidencia::LEVE;

    int    dia    = 1;
    size_t posDia = tokenDia.find('=');
    if (posDia != string::npos)
        dia = stoi(tokenDia.substr(posDia + 1));

    string descripcion = "Incidencia reportada para el equipo " + idEquipoOut;
    return new Incidencia(tipo, descripcion, dia);
}

// ================================================================
// Algoritmos propios
// ================================================================

int Sistema::particionEquipos(int low, int high)
{
    double pivotPrioridad = inventario[high]->calcularPrioridad();
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (inventario[j]->calcularPrioridad() >= pivotPrioridad) {
            i++;
            Equipo* temp  = inventario[i];
            inventario[i] = inventario[j];
            inventario[j] = temp;
        }
    }

    Equipo* temp      = inventario[i + 1];
    inventario[i + 1] = inventario[high];
    inventario[high]  = temp;

    return i + 1;
}

void Sistema::quickSortEquipos(int low, int high)
{
    if (low < high) {
        int pi = particionEquipos(low, high);
        quickSortEquipos(low,    pi - 1);
        quickSortEquipos(pi + 1, high);
    }
}

Equipo* Sistema::busquedaBinariaEquipo(const string& idBusqueda) const
{
    int izquierda = 0;
    int derecha   = static_cast<int>(inventario.size()) - 1;

    while (izquierda <= derecha) {
        int    medio    = izquierda + (derecha - izquierda) / 2;
        string idActual = inventario[medio]->getID();

        if (idActual == idBusqueda) return inventario[medio];
        if (idActual <  idBusqueda) izquierda = medio + 1;
        else                        derecha   = medio - 1;
    }
    return nullptr;
}

// ================================================================
// Calculos globales
// ================================================================

double Sistema::calcularRiesgoGlobal() const
{
    double total = 0.0;
    for (Equipo* eq : inventario)
        total += eq->getPesoTotalIncidencias();
    return total;
}

// El backlog es un numero variable — sistema aleatorio, no puede ser fijo.
// Cuenta cuantos equipos tienen al menos una incidencia activa.
int Sistema::calcularBacklogPendiente() const
{
    int count = 0;
    for (Equipo* eq : inventario)
        if (eq->getCantidadIncidencias() > 0) count++;
    return count;
}

// ================================================================
// Pasos internos de cada dia
// ================================================================

void Sistema::degradarTodos() const
{
    for (Equipo* eq : inventario)
        eq->degradar();
}

void Sistema::activarIncidenciasDia() const
{
    for (Incidencia* inc : historialIncidencias) {
        if (inc->getDiaRegistro() == diaActual && !inc->estaActiva()) {
            Equipo* destino = inc->getEquipo();
            if (destino != nullptr) {
                inc->activar();
                destino->agregarIncidencia(inc);
            }
        }
    }
}

void Sistema::generarIncidenciasAleatorias()
{
    int totalActual = static_cast<int>(historialIncidencias.size());
    if (totalActual >= MAX_INCIDENCIAS) return;

    // Distribuir uniformemente para llegar a exactamente 300 al dia 30
    int diasRestantes = 31 - diaActual;
    int faltantes     = MAX_INCIDENCIAS - totalActual;
    int cuantasHoy    = faltantes / diasRestantes;
    if (cuantasHoy < 1) cuantasHoy = 1;

    uniform_int_distribution<int> distEquipo(0, static_cast<int>(inventario.size()) - 1);
    uniform_int_distribution<int> distTipo(0, 2);

    TipoIncidencia tipos[3] = {
        TipoIncidencia::LEVE,
        TipoIncidencia::MEDIA,
        TipoIncidencia::GRAVE
    };

    for (int i = 0; i < cuantasHoy; i++) {
        Equipo*        destino    = inventario[distEquipo(rng)];
        int            tipoIdx    = distTipo(rng);
        TipoIncidencia tipo       = tipos[tipoIdx];
        const string   tipoNombre[3] = { "LEVE", "MEDIA", "GRAVE" };
        string desc = "Incidencia " + tipoNombre[tipoIdx]
                    + " generada dia " + to_string(diaActual)
                    + " en " + destino->getID();

        auto* nueva = new Incidencia(tipo, desc, diaActual);
        nueva->asignarEquipo(destino);
        nueva->activar();
        destino->agregarIncidencia(nueva);
        historialIncidencias.push_back(nueva);
    }

    cout << "  [Incidencias generadas hoy: " << cuantasHoy
         << " | Total historial: " << historialIncidencias.size() << "]\n";
}

void Sistema::seleccionarYMantener(vector<Equipo*>& atendidos) const
{
    int limite = min(3, static_cast<int>(inventario.size()));
    for (int i = 0; i < limite; i++)
        atendidos.push_back(inventario[i]);

    MantenimientoPreventivo prev;
    MantenimientoCorrectivo corr;
    MantenimientoEmergencia emer;

    for (Equipo* eq : atendidos) {
        if (eq->getEstado() < 30.0 || eq->getCantidadIncidencias() > 5)
            eq->setEstrategia(&emer);
        else if (eq->getEstado() < 60.0)
            eq->setEstrategia(&corr);
        else
            eq->setEstrategia(&prev);

        eq->aplicarMantenimiento();

        // Downcasting seguro — requisito obligatorio del enunciado
        auto* eqCritico = dynamic_cast<EquipoCritico*>(eq);
        if (eqCritico != nullptr)
            eqCritico->calibrarMotor();
    }
}

void Sistema::actualizarTiemposInactivos(const vector<Equipo*>& atendidos) const
{
    for (Equipo* eq : inventario) {
        bool fueAtendido = false;
        for (Equipo* at : atendidos)
            if (at->getID() == eq->getID()) { fueAtendido = true; break; }
        if (!fueAtendido) eq->aumentarTiempoInactivo();
    }
}

// ================================================================
// RF1 — cargarDatosIniciales
// ================================================================

void Sistema::cargarDatosIniciales(const string& rutaArchivo)
{
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
        throw ArchivoInvalidoException("No se pudo abrir: " + rutaArchivo);

    string linea;
    int    numLinea = 0;
    vector<string> lineasIncidencias;

    // Paso 1: leer equipos
    while (getline(archivo, linea)) {
        numLinea++;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty() || linea[0] == '#') continue;

        Equipo* eq = nullptr;
        try {
            if (linea.find("INC") == 0) {
                lineasIncidencias.push_back(linea);
            } else {
                eq = fabricarEquipo(linea);
                inventario.push_back(eq);
            }
        } catch (const SistemaException& e) {
            delete eq;
            cerr << "  [ADVERTENCIA] Linea " << numLinea
                 << " ignorada: " << e.what() << "\n";
        }
    }
    archivo.close();

    // Paso 2: ordenar inventario por ID (necesario para busqueda binaria)
    int n = static_cast<int>(inventario.size());
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (inventario[j]->getID() > inventario[j + 1]->getID()) {
                Equipo* temp    = inventario[j];
                inventario[j]   = inventario[j + 1];
                inventario[j+1] = temp;
            }

    // Paso 3: vincular incidencias usando busqueda binaria
    for (const string& lineaInc : lineasIncidencias) {
        Incidencia* inc = nullptr;
        try {
            string idEquipo;
            inc = fabricarIncidencia(lineaInc, idEquipo);

            Equipo* destino = busquedaBinariaEquipo(idEquipo);
            if (destino != nullptr) {
                inc->asignarEquipo(destino);
            } else {
                cerr << "  [ADVERTENCIA] Equipo '" << idEquipo
                     << "' no encontrado. Incidencia ignorada.\n";
                delete inc;
                continue;
            }
            historialIncidencias.push_back(inc);

        } catch (const SistemaException& e) {
            delete inc;
            cerr << "  [ADVERTENCIA] Error procesando incidencia: " << e.what() << "\n";
        } catch (...) {
            delete inc;
            cerr << "  [ADVERTENCIA] Error inesperado procesando incidencia.\n";
        }
    }

    cout << "  Carga completa y optimizada: "
         << inventario.size()           << " equipos ordenados por ID, "
         << historialIncidencias.size() << " incidencias vinculadas.\n";
}

// ================================================================
// procesarDia — un ciclo completo (RF3 a RF8)
// ================================================================

void Sistema::procesarDia()
{
    cout << "\n--- Dia " << diaActual << " ---\n";

    degradarTodos();
    activarIncidenciasDia();
    generarIncidenciasAleatorias();
    quickSortEquipos(0, static_cast<int>(inventario.size()) - 1);

    vector<Equipo*> atendidos;
    seleccionarYMantener(atendidos);
    actualizarTiemposInactivos(atendidos);
    generarReporteDiario(atendidos);

    diaActual++;
}

// ================================================================
// ejecutarSimulacion — RF2: exactamente 30 dias
// ================================================================

void Sistema::ejecutarSimulacion()
{
    if (inventario.empty())
        throw OperacionInconsistenteException(
            "El inventario esta vacio. Cargue datos antes de simular.");

    cout << "\n===================================================\n";
    cout << "  INICIANDO SIMULACION DE 30 DIAS\n";
    cout << "  Equipos: "     << inventario.size()
         << " | Incidencias: " << historialIncidencias.size() << "\n";
    cout << "===================================================\n";

    while (diaActual <= 30)
        procesarDia();

    double riesgo  = calcularRiesgoGlobal();

    string nivelRiesgo;
    if      (riesgo >= 50) nivelRiesgo = "CRITICO";
    else if (riesgo >= 25) nivelRiesgo = "ALTO";
    else if (riesgo >= 10) nivelRiesgo = "MEDIO";
    else                   nivelRiesgo = "BAJO";

    cout << "\n===================================================\n";
    cout << "  SIMULACION COMPLETADA\n";
    cout << "  Riesgo global final : " << nivelRiesgo    << "\n";
    cout << "  Backlog pendiente   : " << calcularBacklogPendiente() << "\n";
    cout << "  Reportes en        : reporte_dia_01.txt ... reporte_dia_30.txt\n";
    cout << "===================================================\n";
}

// ================================================================
// generarReporteDiario — RF9 + RF10: archivo independiente por dia
// ================================================================

void Sistema::generarReporteDiario(const vector<Equipo*>& atendidos) const
{
    double riesgo  = calcularRiesgoGlobal();
    int    backlog = calcularBacklogPendiente();

    string nivelRiesgo;
    if      (riesgo >= 50) nivelRiesgo = "CRITICO";
    else if (riesgo >= 25) nivelRiesgo = "ALTO";
    else if (riesgo >= 10) nivelRiesgo = "MEDIO";
    else                   nivelRiesgo = "BAJO";

    // --- Consola: formato del enunciado ---
    cout << "Top prioridad : ";
    for (int i = 0; i < static_cast<int>(atendidos.size()); i++) {
        cout << atendidos[i]->getID()
             << " (" << atendidos[i]->calcularPrioridad() << ")";
        if (i < static_cast<int>(atendidos.size()) - 1) cout << " , ";
    }
    cout << "\nAsignados : ";
    for (int i = 0; i < static_cast<int>(atendidos.size()); i++) {
        cout << atendidos[i]->getID();
        if (i < static_cast<int>(atendidos.size()) - 1) cout << " , ";
    }
    cout << "\nBacklog pendiente : " << backlog
         << "\nRiesgo global : "     << nivelRiesgo << "\n";

    // --- Archivo individual por dia ---
    string nombreArchivo = "reporte_dia_";
    if (diaActual < 10) nombreArchivo += "0";
    nombreArchivo += to_string(diaActual) + ".txt";

    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "  [ADVERTENCIA] No se pudo crear " << nombreArchivo << "\n";
        return;
    }

    archivo << "====================================================\n";
    archivo << "  REPORTE DIA " << diaActual << "\n";
    archivo << "====================================================\n";
    archivo << "Dia " << diaActual << "\n";

    archivo << "Top prioridad : ";
    for (int i = 0; i < static_cast<int>(atendidos.size()); i++) {
        archivo << atendidos[i]->getID()
                << " (" << atendidos[i]->calcularPrioridad() << ")";
        if (i < static_cast<int>(atendidos.size()) - 1) archivo << " , ";
    }
    archivo << "\nAsignados : ";
    for (int i = 0; i < static_cast<int>(atendidos.size()); i++) {
        archivo << atendidos[i]->getID();
        if (i < static_cast<int>(atendidos.size()) - 1) archivo << " , ";
    }
    archivo << "\nBacklog pendiente : " << backlog
            << "\nRiesgo global : "     << nivelRiesgo << "\n";

    archivo << "\n--- Detalle equipos atendidos ---\n";
    for (Equipo* eq : atendidos)
        archivo << "  " << eq->getID()
                << " | prioridad="   << eq->calcularPrioridad()
                << " | estado="      << eq->getEstado()
                << " | incidencias=" << eq->getCantidadIncidencias() << "\n";

    archivo << "\n--- Equipos pendientes con incidencias ---\n";
    for (Equipo* eq : inventario) {
        bool atendido = false;
        for (Equipo* at : atendidos)
            if (at->getID() == eq->getID()) { atendido = true; break; }
        if (!atendido && eq->getCantidadIncidencias() > 0)
            archivo << "  " << eq->getID()
                    << " | prioridad="   << eq->calcularPrioridad()
                    << " | estado="      << eq->getEstado()
                    << " | incidencias=" << eq->getCantidadIncidencias() << "\n";
    }

    archivo << "====================================================\n";
    archivo.close();
}