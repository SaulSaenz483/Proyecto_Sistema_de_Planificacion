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

Sistema::Sistema(): diaActual(1), rng(random_device{}()){}

Sistema::~Sistema() {
    // Liberar Equipos
    for (Equipo* eq : inventario) {
        delete eq;
    }
    // Liberar Incidencias
    for (Incidencia* inc : historialIncidencias) {
        delete inc;
    }
    // Cerrar archivo en caso de estar abierto
    if (archivoReporte.is_open()) {
        archivoReporte.close();
    }
}

// Fabricas

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

Incidencia* Sistema::fabricarIncidencia(const string& linea, string& idEquipoOut)
{
    stringstream ss(linea);
    string prefijo, tokenSeveridad, tokenDia;

    // Ejemplo: INC;EQ-001;severidad=ALTA;dia=3

    getline(ss, prefijo,';');
    getline(ss, idEquipoOut,';');
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

    string descripcion = "Incidencia reportada para el equipo " + idEquipoOut;

    //Fabricamos la incidencia

    Incidencia* nuevaInc = new Incidencia(tipo,descripcion,dia);

    return nuevaInc;

}

// Algoritmo

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

// Calculos globales

double Sistema::calcularRiesgoGlobal() const
{
    double riesgoTotal = 0.0;

    //Recorremos el historial completo de incidencias

    for (Equipo* eq :inventario)
    {
        riesgoTotal+= eq->getPesoTotalIncidencias();
    }
    return riesgoTotal;
}


int Sistema::calcularBacklogPendiente() const {
    int totalIncidencias = 0;

    for (Equipo* eq: inventario)
    {
        totalIncidencias += eq->getCantidadIncidencias();
    }

    return totalIncidencias;

}

void Sistema::degradarTodos()
{
    for (Equipo* eq : inventario)
        eq->degradar();
}

// Recorre el historial y activa en cada equipo solo las incidencias
// cuyo diaRegistro coincide con el dia actual de la simulacion.
void Sistema::activarIncidenciasDia()
{
    for (Incidencia* inc : historialIncidencias) {
        if (inc->getDiaRegistro() == diaActual && !inc->estaActiva()) {
            Equipo* destino = inc->getEquipo();
            if (destino != nullptr) {
                inc->activar();              // marcar para no agregar dos veces
                destino->agregarIncidencia(inc);
            }
        }
    }
}

void Sistema::seleccionarYMantener(vector<Equipo*>& atendidos)
{
    int limite = min(3, (int)inventario.size());
    for (int i = 0; i < limite; i++) {
        atendidos.push_back(inventario[i]);
    }
    // Primero registrar, luego mantener
    for (Equipo* eq : atendidos) {
        eq->aplicarMantenimiento();
    }
}

void Sistema::actualizarTiemposInactivos(const vector<Equipo*>& atendidos)
{
    for (Equipo* eq : inventario) {
        bool fueAtendido = false;
        for (Equipo* at : atendidos) {
            if (at->getID() == eq->getID()) {
                fueAtendido = true;
                break;
            }
        }
        if (!fueAtendido) eq->aumentarTiempoInactivo();
    }
}

void Sistema::generarIncidenciasAleatorias()
{
    // Cuantas incidencias ya existen en el historial
    int totalActual = (int)historialIncidencias.size();
    if (totalActual >= MAX_INCIDENCIAS) return; // tope alcanzado

    // Cuantas generar hoy (entre 8 y 12, sin pasarse del tope)
    uniform_int_distribution<int> distCantidad(8, 12);
    int cuantasHoy = distCantidad(rng);
    cuantasHoy = min(cuantasHoy, MAX_INCIDENCIAS - totalActual);

    // Distribuciones para elegir equipo y severidad al azar
    uniform_int_distribution<int> distEquipo(0, (int)inventario.size() - 1);
    uniform_int_distribution<int> distTipo(0, 2); // 0=LEVE, 1=MEDIA, 2=GRAVE

    // Tabla de tipos para construir la descripcion
    TipoIncidencia tipos[3] = {
        TipoIncidencia::LEVE,
        TipoIncidencia::MEDIA,
        TipoIncidencia::GRAVE
    };
    string nombresTipo[3] = { "LEVE", "MEDIA", "GRAVE" };

    int generadas = 0;
    for (int i = 0; i < cuantasHoy; i++) {
        Equipo*        equipoDestino = inventario[distEquipo(rng)];
        TipoIncidencia tipoElegido   = tipos[distTipo(rng)];
        string         desc = "Incidencia " + nombresTipo[(int)tipoElegido]
                            + " generada dia " + to_string(diaActual)
                            + " en " + equipoDestino->getID();

        // Creamos la incidencia con diaRegistro = diaActual
        Incidencia* nueva = new Incidencia(tipoElegido, desc, diaActual);
        nueva->asignarEquipo(equipoDestino);

        // La activamos inmediatamente en el equipo
        nueva->activar();
        equipoDestino->agregarIncidencia(nueva);

        // La guardamos en el historial (Sistema es el dueno de la memoria)
        historialIncidencias.push_back(nueva);
        generadas++;
    }

    cout << "  [Incidencias generadas hoy: " << generadas
         << " | Total historial: " << historialIncidencias.size() << "]\n";
}

// Activa en cada equipo las incidencias del archivo que corresponden a hoy.
// Las incidencias aleatorias ya se activan directamente en generarIncidenciasAleatorias.


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

    while (getline(archivo, linea)) {
        numLinea++;

        // Limpiar \r en caso de archivos con saltos de linea Windows
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty() || linea[0] == '#') continue;

        try {
            if (linea.find("INC") == 0) {
                // --- Incidencia ---
                string      idEquipo;
                Incidencia* inc = fabricarIncidencia(linea, idEquipo);

                // Buscar el equipo con busqueda lineal (inventario aun no esta
                // ordenado por ID en este momento, sino por orden de insercion).
                Equipo* destino = nullptr;
                for (Equipo* eq : inventario) {
                    if (eq->getID() == idEquipo) { destino = eq; break; }
                }

                // Vinculamos la incidencia con su equipo para que activarIncidenciasDia
                // pueda encontrarlo mas tarde. NO la agregamos al equipo todavia.
                if (destino != nullptr) {
                    inc->asignarEquipo(destino);
                } else {
                    cerr << "  [ADVERTENCIA] Linea " << numLinea
                         << ": equipo '" << idEquipo << "' no encontrado. Incidencia guardada sin equipo.\n";
                }

                historialIncidencias.push_back(inc);

            } else {
                // --- Equipo ---
                Equipo* eq = fabricarEquipo(linea);
                inventario.push_back(eq);
            }

        } catch (const SistemaException& e) {
            cerr << "  [ADVERTENCIA] Linea " << numLinea
                 << " ignorada: " << e.what() << "\n";
        }
    }

    archivo.close();

    cout << "  Carga completa: "
         << inventario.size()          << " equipos, "
         << historialIncidencias.size()<< " incidencias en historial.\n";
}

// ================================================================
// procesarDia — un ciclo completo (RF3 a RF8)
// ================================================================

void Sistema::procesarDia()
{
    cout << "\n--- Dia " << diaActual << " ---\n";

    // RF3: todos los equipos se degradan
    degradarTodos();

    // RF2 (paso 2): aparecen las incidencias programadas para hoy
    activarIncidenciasDia();

    // RF4 + RF5: calcular prioridad y ordenar de mayor a menor
    quickSortEquipos(0, (int)inventario.size() - 1);

    // RF6 + RF7: seleccionar top 3 y aplicar mantenimiento
    vector<Equipo*> atendidos;
    seleccionarYMantener(atendidos);

    // RF8: actualizar tiempos inactivos del resto
    actualizarTiemposInactivos(atendidos);

    // RF9 + RF10: reporte del dia
    generarReporteDiario(atendidos);

    diaActual++;
}

// ================================================================
// ejecutarSimulacion — RF2: exactamente 30 dias
// ================================================================

void Sistema::ejecutarSimulacion()
{
    if (inventario.empty())
        throw OperacionInconsistenteException("El inventario esta vacio. Cargue datos antes de simular.");

    archivoReporte.open("reporte_simulacion.txt");
    if (!archivoReporte.is_open())
        throw ArchivoInvalidoException("No se pudo crear reporte_simulacion.txt");

    archivoReporte << "====================================================\n";
    archivoReporte << "  REPORTE DE SIMULACION - SISTEMA DE MANTENIMIENTO\n";
    archivoReporte << "====================================================\n";
    archivoReporte << "Equipos    : " << inventario.size()           << "\n";
    archivoReporte << "Incidencias: " << historialIncidencias.size() << "\n";
    archivoReporte << "====================================================\n\n";

    cout << "\n===================================================\n";
    cout << "  INICIANDO SIMULACION DE 30 DIAS\n";
    cout << "  Equipos: "    << inventario.size()
         << " | Incidencias: " << historialIncidencias.size() << "\n";
    cout << "===================================================\n";

    while (diaActual <= 30)
        procesarDia();

    // Resumen final
    archivoReporte << "\n====================================================\n";
    archivoReporte << "  FIN DE LA SIMULACION\n";
    archivoReporte << "  Riesgo global final : " << calcularRiesgoGlobal()    << "\n";
    archivoReporte << "  Backlog final       : " << calcularBacklogPendiente() << "\n";
    archivoReporte << "====================================================\n";
    archivoReporte.close();

    cout << "\n===================================================\n";
    cout << "  SIMULACION COMPLETADA\n";
    cout << "  Riesgo global final : " << calcularRiesgoGlobal()    << "\n";
    cout << "  Backlog pendiente   : " << calcularBacklogPendiente() << "\n";
    cout << "  Reporte en         : reporte_simulacion.txt\n";
    cout << "===================================================\n";
}

// ================================================================
// generarReporteDiario — RF9 + RF10
// ================================================================

void Sistema::generarReporteDiario(const vector<Equipo*>& atendidos)
{
    double riesgo = calcularRiesgoGlobal();
    string nivelRiesgo;
    if      (riesgo >= 50) nivelRiesgo = "CRITICO";
    else if (riesgo >= 25) nivelRiesgo = "ALTO";
    else if (riesgo >= 10) nivelRiesgo = "MEDIO";
    else                   nivelRiesgo = "BAJO";

    // --- Consola ---
    cout << "Top prioridad: ";
    for (int i = 0; i < (int)atendidos.size(); i++) {
        cout << atendidos[i]->getID()
             << " (" << atendidos[i]->calcularPrioridad() << ")";
        if (i < (int)atendidos.size() - 1) cout << ", ";
    }
    cout << "\n";
    cout << "Asignados    : ";
    for (int i = 0; i < (int)atendidos.size(); i++) {
        cout << atendidos[i]->getID();
        if (i < (int)atendidos.size() - 1) cout << ", ";
    }
    cout << "\n";
    cout << "Backlog      : " << calcularBacklogPendiente() << "\n";
    cout << "Riesgo global: " << nivelRiesgo << " (" << riesgo << ")\n";

    // --- Archivo ---
    if (!archivoReporte.is_open()) return;

    archivoReporte << "----------------------------------------------------\n";
    archivoReporte << "DIA " << diaActual << "\n";
    archivoReporte << "----------------------------------------------------\n";

    archivoReporte << "Equipos atendidos:\n";
    for (Equipo* eq : atendidos) {
        archivoReporte << "  - " << eq->getID()
                       << " | prioridad="   << eq->calcularPrioridad()
                       << " | estado="      << eq->getEstado()
                       << " | incidencias=" << eq->getCantidadIncidencias()
                       << "\n";
    }

    archivoReporte << "Equipos pendientes:\n";
    for (Equipo* eq : inventario) {
        bool atendido = false;
        for (Equipo* at : atendidos)
            if (at->getID() == eq->getID()) { atendido = true; break; }
        if (!atendido) {
            archivoReporte << "  - " << eq->getID()
                           << " | prioridad=" << eq->calcularPrioridad()
                           << " | estado="    << eq->getEstado()
                           << "\n";
        }
    }

    archivoReporte << "Backlog total : " << calcularBacklogPendiente() << "\n";
    archivoReporte << "Riesgo global : " << nivelRiesgo << " (" << riesgo << ")\n\n";
}
