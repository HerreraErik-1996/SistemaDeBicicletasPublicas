#pragma once
#include <string>
#include <iostream>
#include "Estacion.h"

// Nodo de la lista enlazada para manejar colisiones
struct NodoHash
{
    Estacion *estacion;
    NodoHash *siguiente;

    NodoHash(Estacion *e) : estacion(e), siguiente(nullptr) {}
};

class TablaHashEstaciones
{
private:
    NodoHash **tabla; // Array dinámico de punteros a NodoHash
    int capacidad;    // Tamaño M de la tabla (preferentemente número primo, ej: 17 o 31)
    int cantidad;     // Cantidad actual de estaciones (N)
    int colisiones;   // Contador de colisiones para el README

    int funcionHash(const std::string &codigo) const;

public:
    TablaHashEstaciones(int cap = 17);
    ~TablaHashEstaciones();

    bool insertar(Estacion *estacion);
    Estacion *buscar(const std::string &codigo);
    bool eliminar(const std::string &codigo);

    double obtenerFactorDeCarga() const;
    int obtenerColisiones() const;
    int obtenerCantidad() const;
    void mostrarColisiones() const;
};