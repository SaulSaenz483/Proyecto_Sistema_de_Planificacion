#include <iostream>
#include "Equipo.h"
#include "EquipoCritico.h"
#include "EquipoEstandar.h"
#include <vector>
using namespace std;

int main()
{

    cout << "Nada se ha caido" << endl;

    //Prueba de polimorfismo y downcasting

    vector<Equipo*> misEquipos;

    EquipoCritico* equipo1 = new EquipoCritico("22",5,40.0);
    EquipoEstandar* equipo2 = new EquipoEstandar("33",3,20.2);
    misEquipos.push_back(equipo1);
    misEquipos.push_back(equipo2);

    for (int i =0;i<misEquipos.size();i++) {

        misEquipos[i]->aplicarMantenimiento();

        EquipoCritico* punteroCritico = dynamic_cast<EquipoCritico*>(misEquipos[i]);

        if (punteroCritico!= nullptr) {
            cout << "Confirmado, soy critico."<<endl;
        } else {
            cout <<"No soy critico, soy estandar, o de otro tipo"<<endl;
        }

    }
    delete equipo1;
    delete equipo2;
    return 0;
}