#include <iostream>
#include <vector>
#include "Equipo.h"
#include "EquipoCritico.h"
#include "EquipoEstandar.h"
#include "Incidencia.h"
#include "Excepciones.h"
using namespace std;

// Funcion auxiliar para imprimir separadores
void seccion(const string& titulo) {
    cout << "\n========================================" << endl;
    cout << "  " << titulo << endl;
    cout << "========================================" << endl;
}

int main() {

    // -------------------------------------------------------
    // 1. POLIMORFISMO Y DOWNCASTING (tu prueba original)
    // -------------------------------------------------------
    seccion("1. Polimorfismo y downcasting");

    vector<Equipo*> misEquipos;

    EquipoCritico* equipo1 = new EquipoCritico("EQ-001", 8, 75.0);
    EquipoEstandar* equipo2 = new EquipoEstandar("EQ-002", 3, 90.0);
    misEquipos.push_back(equipo1);
    misEquipos.push_back(equipo2);

    for (int i = 0; i < (int)misEquipos.size(); i++) {
        misEquipos[i]->aplicarMantenimiento();

        EquipoCritico* punteroCritico = dynamic_cast<EquipoCritico*>(misEquipos[i]);
        if (punteroCritico != nullptr) {
            cout << "  -> Confirmado: " << misEquipos[i]->getID() << " es CRITICO." << endl;
        } else {
            cout << "  -> Confirmado: " << misEquipos[i]->getID() << " es ESTANDAR." << endl;
        }
    }

    // -------------------------------------------------------
    // 2. INCIDENCIAS — getPeso() y getTipoString()
    // -------------------------------------------------------
    seccion("2. Incidencias: tipos y pesos");

    Incidencia inc1(TipoIncidencia::LEVE,  "Ruido leve en ventilador", 1);
    Incidencia inc2(TipoIncidencia::MEDIA, "Sobrecalentamiento intermitente", 2);
    Incidencia inc3(TipoIncidencia::GRAVE, "Falla en fuente de poder", 3);

    cout << "  Incidencia 1: " << inc1.getTipoString() << " | peso = " << inc1.getPeso() << endl;
    cout << "  Incidencia 2: " << inc2.getTipoString() << " | peso = " << inc2.getPeso() << endl;
    cout << "  Incidencia 3: " << inc3.getTipoString() << " | peso = " << inc3.getPeso() << endl;

    // -------------------------------------------------------
    // 3. CONEXION Incidencia <-> Equipo
    // -------------------------------------------------------
    seccion("3. Conexion Incidencia <-> Equipo");

    equipo1->agregarIncidencia(&inc1);
    equipo1->agregarIncidencia(&inc2);
    equipo1->agregarIncidencia(&inc3);

    inc1.asignarEquipo(equipo1);
    inc2.asignarEquipo(equipo1);
    inc3.asignarEquipo(equipo1);

    cout << "  Equipo: " << equipo1->getID() << endl;
    cout << "  Cantidad de incidencias : " << equipo1->getCantidadIncidencias() << endl;
    cout << "  Peso total incidencias  : " << equipo1->getPesoTotalIncidencias() << endl;
    cout << "  Equipo asociado en inc1 : " << inc1.getEquipo()->getID() << endl;

    // -------------------------------------------------------
    // 4. TIEMPO INACTIVO
    // -------------------------------------------------------
    seccion("4. Tiempo inactivo");

    cout << "  Tiempo inicial de EQ-002: " << equipo2->getTiempoInactivo() << endl;
    equipo2->aumentarTiempoInactivo();
    equipo2->aumentarTiempoInactivo();
    equipo2->aumentarTiempoInactivo();
    cout << "  Despues de 3 dias sin atencion: " << equipo2->getTiempoInactivo() << endl;
    equipo2->resetTiempoInactivo();
    cout << "  Despues de reset: " << equipo2->getTiempoInactivo() << endl;

    // -------------------------------------------------------
    // 5. FORMULA DE PRIORIDAD RF4
    // -------------------------------------------------------
    seccion("5. Formula de prioridad RF4");

    // prioridad = (criticidad * 0.5) + (pesoIncidencias * 0.3) + (tiempoInactivo * 0.2)
    auto calcularPrioridad = [](Equipo* e) -> double {
        return (e->getCriticidad()          * 0.5)
             + (e->getPesoTotalIncidencias()* 0.3)
             + (e->getTiempoInactivo()      * 0.2);
    };

    equipo2->aumentarTiempoInactivo(); // 1 dia inactivo para que haya diferencia

    double p1 = calcularPrioridad(equipo1);
    double p2 = calcularPrioridad(equipo2);

    cout << "  " << equipo1->getID()
         << " | criticidad=" << equipo1->getCriticidad()
         << " | pesoInc=" << equipo1->getPesoTotalIncidencias()
         << " | inactivo=" << equipo1->getTiempoInactivo()
         << " | PRIORIDAD = " << p1 << endl;

    cout << "  " << equipo2->getID()
         << " | criticidad=" << equipo2->getCriticidad()
         << " | pesoInc=" << equipo2->getPesoTotalIncidencias()
         << " | inactivo=" << equipo2->getTiempoInactivo()
         << " | PRIORIDAD = " << p2 << endl;

    cout << "\n  Equipo con mayor prioridad: "
         << (p1 >= p2 ? equipo1->getID() : equipo2->getID()) << endl;

    // -------------------------------------------------------
    // 6. EXCEPCIONES PERSONALIZADAS
    // -------------------------------------------------------
    seccion("6. Excepciones personalizadas");

    // FormatoInvalidoException — descripcion vacia
    try {
        Incidencia mala(TipoIncidencia::LEVE, "", 5);
    } catch (const FormatoInvalidoException& e) {
        cout << "  [OK] Capturada FormatoInvalidoException: " << e.what() << endl;
    }

    // FormatoInvalidoException — dia fuera de rango
    try {
        Incidencia mala(TipoIncidencia::GRAVE, "Error", 99);
    } catch (const FormatoInvalidoException& e) {
        cout << "  [OK] Capturada FormatoInvalidoException: " << e.what() << endl;
    }

    // OperacionInconsistenteException — incidencia nula en equipo
    try {
        equipo1->agregarIncidencia(nullptr);
    } catch (const OperacionInconsistenteException& e) {
        cout << "  [OK] Capturada OperacionInconsistenteException: " << e.what() << endl;
    }

    // OperacionInconsistenteException — asignar equipo nulo a incidencia
    try {
        inc1.asignarEquipo(nullptr);
    } catch (const OperacionInconsistenteException& e) {
        cout << "  [OK] Capturada OperacionInconsistenteException: " << e.what() << endl;
    }

    // FormatoInvalidoException — equipo con datos invalidos
    try {
        EquipoCritico malo("", 5, 50.0); // ID vacio
    } catch (const FormatoInvalidoException& e) {
        cout << "  [OK] Capturada FormatoInvalidoException: " << e.what() << endl;
    }

    // Captura por clase base SistemaException
    try {
        Incidencia mala(TipoIncidencia::MEDIA, "", 1);
    } catch (const SistemaException& e) {
        cout << "  [OK] Capturada por clase base SistemaException: " << e.what() << endl;
    }

    // -------------------------------------------------------
    // Limpieza
    // -------------------------------------------------------
    seccion("Fin de pruebas - todo OK");

    delete equipo1;
    delete equipo2;
    return 0;
}