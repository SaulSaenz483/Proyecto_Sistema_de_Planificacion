//
// Created by Daniel Castillo on 27/04/2026.
//

#include "Estrategias.h"
#include "Equipo.h"
#include <iostream>

using namespace std;

void MantenimientoPreventivo::ejecutarMantenimiento(Equipo* eq) {
    // Sube el estado un 15% (máximo 100)
    double nuevoEstado = eq->getEstado() + 15.0;
    if (nuevoEstado > 100.0) nuevoEstado = 100.0;
    eq->setEstado(nuevoEstado);

    eq->limpiarIncidencias();
    eq->resetTiempoInactivo();
    cout << "    -> Aplicado Mantenimiento PREVENTIVO a " << eq->getID() << "\n";
}

void MantenimientoCorrectivo::ejecutarMantenimiento(Equipo* eq) {
    // Sube el estado un 30%
    double nuevoEstado = eq->getEstado() + 30.0;
    if (nuevoEstado > 100.0) nuevoEstado = 100.0;
    eq->setEstado(nuevoEstado);

    eq->limpiarIncidencias();
    eq->resetTiempoInactivo();
    cout << "  -> Aplicado Mantenimiento CORRECTIVO a " << eq->getID() << "\n";
}

void MantenimientoEmergencia::ejecutarMantenimiento(Equipo* eq) {
    // Restauración total de emergencia
    eq->setEstado(100.0);
    eq->limpiarIncidencias();
    eq->resetTiempoInactivo();
    cout << "     -> Aplicado Mantenimiento DE EMERGENCIA a " << eq->getID() << "\n";
}