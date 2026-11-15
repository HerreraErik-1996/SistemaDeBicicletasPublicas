#pragma once
#include <string>
#include <iostream>
#include "estacion.h"

// Nodo de la lista enlazada para manejar colisiones
struct NodoHash {
    Estacion *estacion;
    NodoHash *siguiente;

    NodoHash(Estacion *e) : estacion(e), siguiente(nullptr) {}
};

class TablaHashEstaciones
{
private:
    NodoHash **tabla; // Array dinámico de punteros a NodoHash
    int capacidad;    // Tamaña M de la tabla (preferentemente número primo, ej: 17 o 31)
    int cantidad;     // Cantidad actual de estaciones (N)
    int colisiones;   // Contador de colisiones para el README

    int funcionHash(const std::string &codigo) const
    {
        unsigned long hash = 5381;
        for (char c : codigo)
        {
            hash = ((hash << 5) + hash) + c; // hash * 32 + c
        }
        return static_cast<int>(hash % capacidad);
    }

public:
    TablaHashEstaciones(int cap = 17)
    {
        capacidad = cap;
        cantidad = 0;
        colisiones = 0;

        tabla = new NodoHash *[capacidad];
        for (int i = 0; i < capacidad; ++i)
        {
            tabla[i] = nullptr;
        }
    }

    ~TablaHashEstaciones()
    {
        for (int i = 0; i < capacidad; ++i)
        {
            NodoHash *actual = tabla[i];
            while (actual != nullptr)
            {
                NodoHash *aBorrar = actual;
                actual = actual->siguiente;
                delete aBorrar;
            }
        }
        delete[] tabla;
    }; // Liberar toda la memoria dinámica

    bool insertar(Estacion* estacion) {
        int indice = funcionHash(estacion->getCodigo());

        // Si la celda ya tiene elementos, hubo colisión
        if (tabla[indice] != nullptr)
        {
            colisiones++;
        };
        NodoHash *actual = tabla[indice];

        // Verificar si la estación ya existe en la lista
        while (actual != nullptr)
        {
            if (actual->estacion->getCodigo() == estacion->getCodigo())
            {
                return false; // Código duplicado, no se inserta
            }
            actual = actual->siguiente;
        }
        // Insertar al inicio de la lista enlazada (O(1))
        NodoHash *nuevoNodo = new NodoHash (estacion);
        nuevoNodo->siguiente = tabla[indice];
        tabla[indice] = nuevoNodo;
        cantidad++;

        return true;
    }

    Estacion *buscar(const std::string &codigo)
    {
        int indice = funcionHash(codigo);
        NodoHash *actual = tabla[indice];

        while (actual != nullptr)
        {
            if (actual->estacion->getCodigo() == codigo)
            {
                return (actual->estacion); // Retorna puntero a la estación dentro del nodo
            }
            actual = actual->siguiente;
        }
        return nullptr; // No encontrada
    };

    bool eliminar(const std::string &codigo)
    {
        int indice = funcionHash(codigo);
        NodoHash *actual = tabla[indice];
        NodoHash *anterior = nullptr;

        while (actual != nullptr)
        {
            if (actual->estacion->getCodigo() == codigo)
            {
                if (anterior == nullptr)
                {
                    //es el primer nodo de la lista
                    tabla[indice] = actual->siguiente;
                }
                else
                {
                    //esta en el medio o al final,ejemplo,si es el 2do el que borramos,el 3ro pasa a ser el 2do del primero.
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                cantidad--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    };

    double obtenerFactorDeCarga() const
    {
        return static_cast<double>(cantidad) / capacidad;
    }
    int obtenerColisiones() const
    {
        return colisiones;
    }
    int obtenerCantidad() const
    {
        return cantidad;
    }
};