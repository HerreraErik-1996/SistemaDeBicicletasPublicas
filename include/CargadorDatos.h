#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include "Sistema.h"

class CargadorDatos {
public:

    bool cargarEstaciones(Sistema& sistema) {

        std::ifstream archivo("data/estaciones.txt");

        if (!archivo.is_open()) {
            std::cout << "Error al abrir estaciones.txt\n";
            return false;
        }

        std::string encabezado;
        std::getline(archivo, encabezado);

        std::string codigo;
        std::string nombre;
        std::string barrio;
        int capacidad;
        int disponibles;

        while (archivo >> codigo >> nombre >> barrio>> capacidad >> disponibles) {
            sistema.agregarEstacion(
                codigo,
                nombre,
                barrio,
                capacidad,
                disponibles
            );
        }
        return true;
    }

    bool cargarCiclovias(Sistema& sistema) {

        std::ifstream archivo("data/ciclovias.txt");

        if (!archivo.is_open()) {
            std::cout << "Error al abrir ciclovias.txt\n";
            return false;
        }

        std::string encabezado;
        std::getline(archivo, encabezado); // descartamos el encabezado

        std::string origen;
        std::string destino;

        while (archivo >> origen >> destino) {

            sistema.agregarCiclovia(origen, destino);
        }

        return true;
    }
};