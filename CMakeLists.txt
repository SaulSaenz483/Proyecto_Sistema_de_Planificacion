cmake_minimum_required(VERSION 3.14)
project(Proyecto_Sistema_de_Planificacion)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto_Sistema_de_Planificacion
        main.cpp
        Equipo.cpp
        Equipo.h
        Incidencia.cpp
        Incidencia.h
        EquipoCritico.cpp
        EquipoCritico.h
        EquipoEstandar.cpp
        EquipoEstandar.h
        Excepciones.h
        Sistema.cpp
        Sistema.h
)

# Copiar datos.txt al directorio de build automaticamente cada vez que se compila
configure_file(${CMAKE_SOURCE_DIR}/datos.txt ${CMAKE_BINARY_DIR}/datos.txt COPYONLY)
