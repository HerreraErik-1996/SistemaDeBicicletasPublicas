#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "Grafo.h"
#include "TablaHashEstacion.h"
#include "ArbolBicicletas.h"
#include "ListaViaje.h"
#include "Taller.h"

class Sistema
{
private:
    Grafo redEstaciones;
    TablaHashEstaciones tablaEstaciones;
    ArbolBicicletas arbolBicis;
    ListaViajes listaViajes;
    MinHeapTaller tallerReparaciones;

    // Métodos de carga
    bool cargarBicicletas();
    bool cargarEstaciones();
    bool registrarNuevaEstacion(const std::string &codigo, const std::string &nombre, const std::string &barrio, int capacidad);
    bool eliminarEstacion(const std::string &codigo);
    bool cargarCiclovias();
    bool cargarViajes();
    void registrarNuevoViaje(int idViaje, const string &dniUsuario, const string &codOrigen, const string &codDestino, int fecha, int duracionMin, int kmRecorridos);

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
    void ejecutarRetirarDevolverBici();
    string uppercase(string *texto);

  public:
    Sistema();
    ~Sistema();

    void ejecutar();
};