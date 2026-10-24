#include "viaje.h"
#include <iostream>
#include <fstream>
using namespace std;

ListaViajes::ListaViajes()
{
    cabeza = nullptr;
    cantidad = 0;
}

// Destructor: Recorre la lista y destruye cada nodo reservado con new
ListaViajes::~ListaViajes()
{
    NodoViaje *actual = cabeza;
    while (actual != nullptr)
    {
        NodoViaje *aBorrar = actual;
        actual = actual->siguiente;
        delete aBorrar; // Libera nodo por nodo
    }
}

// Inserta al inicio de la lista
void ListaViajes::agregarViaje(const Viaje &nuevoViaje)
{
    NodoViaje *nuevoNodo = new NodoViaje;
    nuevoNodo->datos = nuevoViaje;
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    cantidad++;
}

// Lectura del archivo viajes.txt
void ListaViajes::cargarDesdeArchivo(const string &rutaArchivo)
{
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << rutaArchivo << endl;
        return;
    }

    Viaje v;
    // Lee espacio por espacio según el formato del archivo
    while (archivo >> v.viajeId >> v.biciId >> v.dniUsuario >> v.estacionOrigen >> v.estacionDestino >> v.fecha >> v.duracionMin)
    {
        agregarViaje(v);
    }
    archivo.close();
}

// Punto B.1: Conteo de viajes en rango de fechas
int ListaViajes::contarViajesPorEstacionYRango(const string &codigoEstacion, int fechaInicio, int fechaFin)
{
    int contador = 0;
    NodoViaje *actual = cabeza;

    while (actual != nullptr)
    {
        if (actual->datos.estacionOrigen == codigoEstacion &&
            actual->datos.fecha >= fechaInicio &&
            actual->datos.fecha <= fechaFin)
        {
            contador++;
        }
        actual = actual->siguiente;
    }

    return contador;
}

// Punto B.2: Búsqueda de viajes por DNI y suma total de minutos
void ListaViajes::buscarViajesPorDNI(const string &dniBuscado)
{
    int totalMinutos = 0;
    bool encontrado = false;
    NodoViaje *actual = cabeza;

    cout << "\nViajes del usuario DNI " << dniBuscado << ":\n";
    cout << "----------------------------------------\n";

    while (actual != nullptr)
    {
        if (actual->datos.dniUsuario == dniBuscado)
        {
            cout << "ID: " << actual->datos.viajeId
                 << " | " << actual->datos.estacionOrigen
                 << " -> " << actual->datos.estacionDestino
                 << " | Fecha: " << actual->datos.fecha
                 << " | Duracion: " << actual->datos.duracionMin << " min\n";

            totalMinutos += actual->datos.duracionMin;
            encontrado = true;
        }
        actual = actual->siguiente;
    }

    if (!encontrado)
    {
        cout << "No se registraron viajes para el DNI " << dniBuscado << ".\n";
    }
    else
    {
        cout << "----------------------------------------\n";
        cout << "Total tiempo pedaleado: " << totalMinutos << " minutos.\n";
    }
}
