#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "grafo.h"
#include "TablaHashEstacion.h"

using namespace std;

class Sistema
{
private:
    Grafo redEstaciones;
    // ArbolBicicletas arbolBicis;   // Descomentar cuando lo integres
    // ListaViajes listaViajes;       // Descomentar cuando lo integres

    // Métodos privados auxiliares de carga de archivos
    bool cargarEstaciones();
    bool cargarCiclovias();

    // Métodos privados para las opciones del menú
    void mostrarMenu() const;
    void ejecutarListadoOrdenado();
    void ejecutarAlertaOcupacion();

public:
    Sistema();
    ~Sistema();

    // Punto de entrada principal
    void ejecutar();
};