//
// Created by Daniel Castillo on 16/04/2026.
//

#include "EquipoCritico.h"


void EquipoCritico::degradar()
{
    double desgaste = 2.5 * criticidad;
    estado -= desgaste;

    if (estado <0)
    {
        estado = 0; //Evitamos que de numeros negativos
    }

}

void EquipoCritico::calibrarMotor()
{
    cout << "->Calibrando motor..."<<endl;
}





