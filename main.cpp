#include <iostream>
#include <vector>
#include "Sistema.h"
#include "Equipo.h"
#include "EquipoCritico.h"
#include "EquipoEstandar.h"
#include "Incidencia.h"
#include "Excepciones.h"
using namespace std;

void seccion(const string& titulo) {
    cout << "\n========================================\n";
    cout << "  " << titulo << "\n";
    cout << "========================================\n";
}

int main()
{
    // -------------------------------------------------------
    // MODO 1: Pruebas unitarias de clases individuales
    // -------------------------------------------------------

    seccion("1. Polimorfismo y downcasting");

    vector<Equipo*> misEquipos;
    EquipoCritico*  equipo1 = new EquipoCritico("EQ-001", 8, 75.0);
    EquipoEstandar* equipo2 = new EquipoEstandar("EQ-002", 3, 90.0);
    misEquipos.push_back(equipo1);
    misEquipos.push_back(equipo2);

    for (int i = 0; i < (int)misEquipos.size(); i++) {
        misEquipos[i]->aplicarMantenimiento();
        EquipoCritico* punteroCritico = dynamic_cast<EquipoCritico*>(misEquipos[i]);
        if (punteroCritico != nullptr)
            cout << "  -> Confirmado: " << misEquipos[i]->getID() << " es CRITICO.\n";
        else
            cout << "  -> Confirmado: " << misEquipos[i]->getID() << " es ESTANDAR.\n";
    }

    seccion("2. Incidencias: tipos y pesos");

    Incidencia inc1(TipoIncidencia::LEVE,  "Ruido leve en ventilador",       1);
    Incidencia inc2(TipoIncidencia::MEDIA, "Sobrecalentamiento intermitente", 2);
    Incidencia inc3(TipoIncidencia::GRAVE, "Falla en fuente de poder",        3);

    cout << "  inc1: " << inc1.getTipoString() << " | peso=" << inc1.getPeso() << "\n";
    cout << "  inc2: " << inc2.getTipoString() << " | peso=" << inc2.getPeso() << "\n";
    cout << "  inc3: " << inc3.getTipoString() << " | peso=" << inc3.getPeso() << "\n";

    seccion("3. Conexion Incidencia <-> Equipo");

    equipo1->agregarIncidencia(&inc1);
    equipo1->agregarIncidencia(&inc2);
    equipo1->agregarIncidencia(&inc3);
    inc1.asignarEquipo(equipo1);
    inc2.asignarEquipo(equipo1);
    inc3.asignarEquipo(equipo1);

    cout << "  Equipo: "             << equipo1->getID() << "\n";
    cout << "  Cantidad incidencias: " << equipo1->getCantidadIncidencias() << "\n";
    cout << "  Peso total:           " << equipo1->getPesoTotalIncidencias() << "\n";
    cout << "  Equipo en inc1:       " << inc1.getEquipo()->getID() << "\n";

    seccion("4. limpiarIncidencias tras mantenimiento");
    cout << "  Incidencias antes del mantenimiento: " << equipo1->getCantidadIncidencias() << "\n";
    equipo1->aplicarMantenimiento();
    cout << "  Incidencias despues del mantenimiento: " << equipo1->getCantidadIncidencias() << "\n";

    seccion("5. Tiempo inactivo");
    cout << "  Tiempo inicial EQ-002: " << equipo2->getTiempoInactivo() << "\n";
    equipo2->aumentarTiempoInactivo();
    equipo2->aumentarTiempoInactivo();
    equipo2->aumentarTiempoInactivo();
    cout << "  Despues de 3 dias sin atencion: " << equipo2->getTiempoInactivo() << "\n";
    equipo2->resetTiempoInactivo();
    cout << "  Despues de reset: " << equipo2->getTiempoInactivo() << "\n";

    seccion("6. Formula de prioridad RF4");
    equipo2->aumentarTiempoInactivo();
    cout << "  " << equipo1->getID()
         << " | criticidad=" << equipo1->getCriticidad()
         << " | pesoInc="    << equipo1->getPesoTotalIncidencias()
         << " | inactivo="   << equipo1->getTiempoInactivo()
         << " | PRIORIDAD="  << equipo1->calcularPrioridad() << "\n";
    cout << "  " << equipo2->getID()
         << " | criticidad=" << equipo2->getCriticidad()
         << " | pesoInc="    << equipo2->getPesoTotalIncidencias()
         << " | inactivo="   << equipo2->getTiempoInactivo()
         << " | PRIORIDAD="  << equipo2->calcularPrioridad() << "\n";

    seccion("7. Excepciones personalizadas");

    try { Incidencia mala(TipoIncidencia::LEVE, "", 5); }
    catch (const FormatoInvalidoException& e) {
        cout << "  [OK] " << e.what() << "\n"; }

    try { Incidencia mala(TipoIncidencia::GRAVE, "X", 99); }
    catch (const FormatoInvalidoException& e) {
        cout << "  [OK] " << e.what() << "\n"; }

    try { equipo1->agregarIncidencia(nullptr); }
    catch (const OperacionInconsistenteException& e) {
        cout << "  [OK] " << e.what() << "\n"; }

    try { inc1.asignarEquipo(nullptr); }
    catch (const OperacionInconsistenteException& e) {
        cout << "  [OK] " << e.what() << "\n"; }

    try { EquipoCritico malo("", 5, 50.0); }
    catch (const FormatoInvalidoException& e) {
        cout << "  [OK] " << e.what() << "\n"; }

    try { Incidencia mala(TipoIncidencia::MEDIA, "", 1); }
    catch (const SistemaException& e) {
        cout << "  [OK] Capturada por SistemaException: " << e.what() << "\n"; }

    delete equipo1;
    delete equipo2;

    // -------------------------------------------------------
    // MODO 2: Simulacion completa con archivo de datos
    // -------------------------------------------------------
    seccion("8. Simulacion completa de 30 dias");

    try {
        // Debug: verificar que el archivo se abre
        ifstream prueba("datos.txt");
        if (!prueba.is_open()) {
            cout << "  [ERROR] No se puede abrir datos.txt\n";
        } else {
            cout << "  [OK] Archivo encontrado. Primeras 3 lineas:\n";
            string linea;
            int cont = 0;
            while (getline(prueba, linea) && cont < 3) {
                cout << "  [" << cont << "] '" << linea << "'\n";
                cont++;
            }
            prueba.close();
        }

        Sistema sistema;
        sistema.cargarDatosIniciales("datos.txt");
        sistema.ejecutarSimulacion();
    } catch (const SistemaException& e) {
        cerr << "[ERROR] " << e.what() << "\n";
        return 1;
    }

    seccion("Fin - todo completado");
    return 0;
}