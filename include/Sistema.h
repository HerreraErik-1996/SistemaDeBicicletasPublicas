#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "grafo.h"
#include "TablaHashEstacion.h"
#include "ArbolBicicletas.h"
#include "viaje.h"
#include "taller.h"

class Sistema
{
private:
    Grafo redEstaciones;
    TablaHashEstaciones tablaEstaciones;
    ArbolBicicletas arbolBicis;
    ListaViajes listaViajes;
    MinHeapTaller tallerReparaciones;

    // Métodos de carga
    bool cargarEstaciones();
    bool cargarCiclovias();
    bool cargarViajes();
    bool cargarBicicletas();

    // Auxiliares de consola
    void limpiarPantalla() const;
    void pausar() const;

    // Submenús modulares
    void menuEstaciones();
    void menuViajesYColas();
    void menuRegistroBicicletas();

    // Acciones auxiliares
    void ejecutarListadoOrdenado();
    void ejecutarListarPorOcupacion();

public:
    Sistema();
    ~Sistema();

    void ejecutar();
};