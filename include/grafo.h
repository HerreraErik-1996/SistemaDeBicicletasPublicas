#pragma once
#include <iostream>
#include "estacion.h"
using namespace std;

class Grafo
{
private:
    struct Nodo
    {
        int idVertice;
        Nodo *siguiente;
    };

    struct Vertice
    {
        int id;
        Estacion *estacion;
        Nodo *adyacentes;
    };

    int cantidadVertices;
    int capacidadVertices;

    Vertice *vertices;

    int buscarId(const string &codigo)
    {
        for (int i = 0; i < cantidadVertices; i++)
        {
            if (vertices[i].estacion->getCodigo() == codigo)
            {
                return i;
            }
        }
        return -1;
    }

public:
    Grafo()
    {
        cantidadVertices = 0;
        capacidadVertices = 0;
        vertices = nullptr;
    }

    void agregarEstacion(string codigo, string nombre, string barrio, int capacidad, int disponibles)
    {
        if (buscarId(codigo) != -1)
        {
            cout << "La estacion " << codigo << " ya existe" << endl;
            return;
        }
        if (cantidadVertices == capacidadVertices)
        {
            int nuevaCap;

            if (capacidadVertices == 0)
            {
                nuevaCap = 2;
            }
            else
            {
                nuevaCap = capacidadVertices * 2;
            }
            Vertice *nuevosVertices = new Vertice[nuevaCap];

            for (int i = 0; i < cantidadVertices; i++)
            {
                nuevosVertices[i] = vertices[i];
            }
            delete[] vertices;
            vertices = nuevosVertices;
            capacidadVertices = nuevaCap;
        }
        Estacion *nuevaEstacion = new Estacion(codigo, nombre, barrio, capacidad, disponibles);

        vertices[cantidadVertices].estacion = nuevaEstacion;
        vertices[cantidadVertices].adyacentes = nullptr;
        cantidadVertices++;
    }

    void agregarArista(const string &origen, const string &destino)
    {
        int idOrigen = buscarId(origen);
        int idDestino = buscarId(destino);
        // verifica que las estaciones existan
        if (idOrigen == -1 || idDestino == -1)
        {
            cout << "Error: alguna de las estaciones no existe." << endl;
            return;
        }
        // evita bucle
        if (idOrigen == idDestino)
        {
            return;
        }
        // evitar aristas duplicadas
        Nodo *actual = vertices[idOrigen].adyacentes;
        while (actual != nullptr)
        {
            if (actual->idVertice == idDestino)
            {
                return;
            }
            actual = actual->siguiente;
        }

        Nodo *nuevo = new Nodo;
        nuevo->idVertice = idDestino;
        nuevo->siguiente = vertices[idOrigen].adyacentes;
        vertices[idOrigen].adyacentes = nuevo;

        // porque es grafo no dirigido
        Nodo *nuevo2 = new Nodo;
        nuevo2->idVertice = idOrigen;
        nuevo2->siguiente = vertices[idDestino].adyacentes;
        vertices[idDestino].adyacentes = nuevo2;
    }

    void mostrar()
    {
        for (int i = 0; i < cantidadVertices; i++)
        {
            cout << "Estacion " << vertices[i].estacion->getCodigo() << ": ";

            Nodo *actual = vertices[i].adyacentes;

            while (actual != nullptr)
            {
                int id = actual->idVertice;
                cout << vertices[id].estacion->getCodigo();
                actual = actual->siguiente;
                if (actual != nullptr)
                {
                    cout << " -> ";
                }
            }
            cout << endl;
        }
    }

    void listarEstacionesOrdenadas(int tipoListado)
    {
        if (this->vertices == nullptr || this->cantidadVertices <= 0)
        {
            cout << "No hay estaciones registradas en el grafo.\n";
            return;
        }

        // 1. Crear copia auxiliar extrayendo el puntero 'estacion' de cada Vertice
        Estacion **listaOrdenada = new Estacion *[this->cantidadVertices];
        for (int i = 0; i < this->cantidadVertices; i++)
        {
            listaOrdenada[i] = this->vertices[i].estacion;
        }

        // 2. Ordenamiento por Inserción (Insertion Sort)
        for (int i = 1; i < this->cantidadVertices; i++)
        {
            Estacion *clave = listaOrdenada[i];
            int j = i - 1;

            while (j >= 0)
            {
                bool debeIntercambiar = false;

                if (tipoListado == 0)
                {
                    // Mayor a menor por Capacidad de Anclajes
                    if (listaOrdenada[j]->getCapacidad() < clave->getCapacidad())
                    {
                        debeIntercambiar = true;
                    }
                }
                else if (tipoListado == 1)
                {
                    // Mayor a menor por Bicicletas Disponibles
                    if (listaOrdenada[j]->getBicisDisp() < clave->getBicisDisp())
                    {
                        debeIntercambiar = true;
                    }
                }

                if (debeIntercambiar)
                {
                    listaOrdenada[j + 1] = listaOrdenada[j];
                    j--;
                }
                else
                {
                    break;
                }
            }
            listaOrdenada[j + 1] = clave;
        }

        // 3. Imprimir por pantalla
        if (tipoListado == 0)
        {
            cout << "\n=== ESTACIONES ORDENADAS POR CAPACIDAD DE ANCLAJES (MAYOR A MENOR) ===\n";
        }
        else
        {
            cout << "\n=== ESTACIONES ORDENADAS POR BICICLETAS DISPONIBLES (MAYOR A MENOR) ===\n";
        }

        for (int i = 0; i < this->cantidadVertices; i++)
        {
            cout << i + 1 << ". ";
            listaOrdenada[i]->mostrarInfo(); // Muestra los datos de la estación
        }

        // 4. Liberar memoria dinámica auxiliar
        delete[] listaOrdenada;
    }

    void listarEstacionesBajaOcupacion()
    {
        if (this->vertices == nullptr || this->cantidadVertices <= 0)
        {
            cout << "No hay estaciones registradas en el sistema.\n";
            return;
        }

        bool hayEstacionesCriticas = false;

        cout << "\n=== ESTACIONES CON MENOS DEL 20% DE OCUPACION ===\n";

        for (int i = 0; i < this->cantidadVertices; i++)
        {
            Estacion *est = this->vertices[i].estacion;

            if (est != nullptr && est->getCapacidad() > 0)
            {
                // Evaluación con enteros: (bicis * 100) < (capacidad * 20)
                if (est->getBicisDisp() * 100 < est->getCapacidad() * 20)
                {
                    int porcentaje = (est->getBicisDisp() * 100) / est->getCapacidad();

                    cout << "- Estacion: " << est->getCodigo()
                         << " | Disponibles: " << est->getBicisDisp()
                         << " / Capacidad: " << est->getCapacidad()
                         << " (" << porcentaje << "% de ocupacion)\n";

                    hayEstacionesCriticas = true;
                }
            }
        }

        if (!hayEstacionesCriticas)
        {
            cout << "Todas las estaciones operan con una ocupacion igual o superior al 20%.\n";
        }
    }

    Grafo(const Grafo &) = delete;
    Grafo &operator=(const Grafo &) = delete;
};