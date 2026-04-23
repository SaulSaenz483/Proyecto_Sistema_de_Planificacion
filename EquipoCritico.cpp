//
// Created by Daniel Castillo on 16/04/2026.
//

#include "EquipoCritico.h"


void EquipoCritico::degradar()
{
    double desgaste = 2.5 * criticidad;
    estado = estado - desgaste;

    if (estado <0)
    {
        estado = 0; //Evitamos que de numeros negativos
    }

}


void EquipoCritico:: aplicarMantenimiento() {
    cout <<" Realizando mantenimiento al equipo critico: " << getID() << endl;

    calibrarMotor();

    estado = 100.0;
    resetTiempoInactivo();
}

void EquipoCritico::calibrarMotor()
{
    cout << "->Calibrando motor..."<<endl;
}





