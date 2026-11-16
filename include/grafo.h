#pragma once
#include <iostream>
#include <string>

#include "estacion.h"

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

    int buscarId(const std::string &codigo);

public:
    Grafo();
    ~Grafo();
    void agregarEstacion(Estacion *est);
    void agregarArista(const std::string &origen, const std::string &destino);
    void mostrar();
    void listarEstacionesOrdenadas(int tipoListado);
    void listarEstacionesBajaOcupacion();
    void BFS(const std::string &origen, const std::string &destino);

    Grafo(const Grafo &) = delete;
    Grafo &operator=(const Grafo &) = delete;
};