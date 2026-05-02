// Sistema de Planificación Inteligente de Mantenimiento
// Daniel Castillo Jiménez
// Saul Sáenz Vargas

#include <iostream>
#include "Sistema.h"
#include "Excepciones.h"
using namespace std;

int main() {
    try {
        Sistema sistema;
        sistema.cargarDatosIniciales("datos.txt");
        sistema.ejecutarSimulacion();
    } catch (const SistemaException& e) {
        cerr << "[ERROR] " << e.what() << endl;
        return 1;
    }
    return 0;
}