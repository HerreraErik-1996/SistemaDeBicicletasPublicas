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

    int buscarId(const string &codigo){
        for (int i = 0; i < cantidadVertices; i++){
            if (vertices[i].estacion->getCodigo() == codigo){
                return i;
            }
        }
        return -1;
    }

public:
    Grafo()
    {
        cantidadVertices = 0;
        capacidadVertices= 0;
        vertices = nullptr;
    }

    void agregarEstacion(string codigo, string nombre, string barrio, int capacidad, int disponibles)
    {
        if (buscarId(codigo) != -1)
        {
            cout << "La estacion " << codigo << " ya existe" << endl;
            return;
        }
        if (cantidadVertices == capacidadVertices){
            int nuevaCap;
            
            if(capacidadVertices == 0){
                nuevaCap = 2;
            }else{
                nuevaCap = capacidadVertices * 2;
            }
            Vertice *nuevosVertices = new Vertice[nuevaCap];

            for (int i = 0; i < cantidadVertices; i++){
                nuevosVertices[i]= vertices[i];
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

    void agregarArista(const string &origen,const string &destino)
    {
        int idOrigen = buscarId(origen);
        int idDestino = buscarId(destino);
        //verifica que las estaciones existan
        if (idOrigen == -1 || idDestino == -1){
            cout << "Error: alguna de las estaciones no existe." << endl;
            return;
        }
        //evita bucle
        if (idOrigen == idDestino)
        {
            return;
        }
        //evitar aristas duplicadas
        Nodo *actual = vertices[idOrigen].adyacentes;
        while (actual != nullptr){
            if (actual->idVertice == idDestino){
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

            while (actual != nullptr){
                int id = actual->idVertice;
                cout << vertices[id].estacion->getCodigo();
                actual = actual->siguiente;
                if (actual != nullptr){
                    cout << " -> ";
                }
            }
            cout << endl;
        }
    }

    
    Grafo(const Grafo &) = delete;
    Grafo &operator=(const Grafo &) = delete;
};