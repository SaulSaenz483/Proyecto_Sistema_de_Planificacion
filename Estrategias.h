//
// Created by Daniel Castillo on 27/04/2026.
//

#ifndef PROYECTO_SISTEMA_DE_PLANIFICACION_ESTRATEGIAS_H
#define PROYECTO_SISTEMA_DE_PLANIFICACION_ESTRATEGIAS_H


// Declaración adelantada para evitar dependencias circulares
class Equipo;

//  LA INTERFAZ (Clase Abstracta)
class EstrategiaMantenimiento {
public:
    virtual ~EstrategiaMantenimiento() = default;
    // El método puro que obliga a las hijas a implementarlo
    virtual void ejecutarMantenimiento(Equipo* eq) = 0;
};

// --- ESTRATEGIA 1: Preventivo ---
class MantenimientoPreventivo : public EstrategiaMantenimiento {
public:
    void ejecutarMantenimiento(Equipo* eq) override;
};

// --- ESTRATEGIA 2: Correctivo ---
class MantenimientoCorrectivo : public EstrategiaMantenimiento {
public:
    void ejecutarMantenimiento(Equipo* eq) override;
};

// --- ESTRATEGIA 3: Emergencia ---
class MantenimientoEmergencia : public EstrategiaMantenimiento {
public:
    void ejecutarMantenimiento(Equipo* eq) override;
};


#endif //PROYECTO_SISTEMA_DE_PLANIFICACION_ESTRATEGIAS_H