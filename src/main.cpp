#pragma once
#include <iostream>
#include <fstream>
#include "Grafo.h"

#include "Sistema.h"
using namespace std;

Grafo redEstaciones;
bool cargarCiclovias()
{
    ifstream archivo("data/ciclovias.txt");

    if (!archivo.is_open())
    {
        cout << "Error al abrir ciclovias.txt\n";
        return false;
    }

    string encabezado;
    getline(archivo, encabezado); // descartamos el encabezado

    string origen;
    string destino;

    while (archivo >> origen >> destino)
    {
        redEstaciones.agregarArista(origen, destino);
    }
    return true;
};

bool cargarEstaciones()
{

    ifstream archivo("data/estaciones.txt");

    if (!archivo.is_open())
    {
        cout << "Error al abrir estaciones.txt\n";
        return false;
    }

    string encabezado;
    getline(archivo, encabezado);

    string codigo;
    string nombre;
    string barrio;
    int capacidad;
    int disponibles;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> disponibles)
    {
        redEstaciones.agregarEstacion(
            codigo,
            nombre,
            barrio,
            capacidad,
            disponibles);
    }
    return true;
}

//sistema.ejecutarMenu();

int main() {
    cargarEstaciones;
    cargarCiclovias;
    if (cargarEstaciones() && cargarCiclovias())
    {
        cout << "\n--- GRAFO DE ESTACIONES ---\n";
        redEstaciones.mostrar();
    }
    return 0;
};