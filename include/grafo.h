#pragma once
#include <iostream>
#include "estacion.h"
using namespace std;

class Grafo
{
private:
    struct Nodo
    {
        int vertice;
        Nodo *siguiente;
    };

    struct Vertice
    {
        Estacion *estacion;
        NodoAdyacente *adyacentes;
    };

    int cantidadVertices;

    Nodo **adyacencia;

public:
    Grafo (int cantidad)
    {
        cantidadVertices = cantidad;
        adyacencia = new Nodo *[cantidadVertices];
        for (int i = 0; i < cantidadVertices; i++){
            adyacencia[i] = nullptr;
        }
    }

    void agregarArista(int origen, int destino)
    {
        if (origen < 0 || origen >= cantidadVertices || destino < 0 || destino >= cantidadVertices){
                return;
            }
        Nodo *nuevo = new Nodo;
        nuevo->vertice= destino;
        nuevo->siguiente= adyacencia[origen];
        adyacencia[origen] = nuevo;
        
        //porque es grafo no dirigido
        Nodo *nuevo2 = new Nodo;
        nuevo2->vertice = destino;
        nuevo2->siguiente = adyacencia[destino];
        adyacencia[destino] = nuevo2;
    }

    void mostrar()
    {
        for (int i = 0; i < cantidadVertices; i++)
        {
            cout << "Vertice " << i << ": ";

            Nodo *actual = adyacencia[i];

            while (actual != nullptr)
            {
                cout << actual->vertice << " ";
                actual = actual->siguiente;
            }

            cout << endl;
        }
    }

    ~Grafo()
    {
        for (int i = 0; i < cantidadVertices; i++)
        {
            Nodo *actual = adyacencia[i];

            while (actual != nullptr)
            {
                Nodo *auxiliar = actual;
                actual = actual->siguiente;
                delete auxiliar;
            }
        }

        delete[] adyacencia;
    }
    Grafo(const Grafo &) = delete;
    Grafo &operator=(const Grafo &) = delete;
};