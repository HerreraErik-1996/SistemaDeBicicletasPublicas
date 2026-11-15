#pragma once
#include <iostream>
#include "bicicleta.h"

struct ElementoTaller
{
    Bicicleta *bici; // Puntero al objeto Bicicleta real
    int prioridad;   // 1: Alta, 2: Media, 3: Baja
};

class MinHeapTaller
{
private:
    ElementoTaller *heap;
    int capacidad;
    int cantidad;

    void flotar(int i);
    void hundir(int i);
    void redimensionar(int nuevaCapacidad);

public:
    MinHeapTaller(int capInicial = 10);
    ~MinHeapTaller();

    // Recibe el puntero a la bicicleta y calcula automáticamente la prioridad si es eléctrica
    void insertarBicicleta(Bicicleta *bici, int prioridadIngresada = 2);

    // Retorna la bicicleta más urgente reparada o para reparar
    Bicicleta *extraerMasUrgente();

    bool estaVacio() const;
    int getCantidad() const;
    void mostrarTaller() const;
};