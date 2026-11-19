#include "../include/taller.h"

using namespace std;

MinHeapTaller::MinHeapTaller(int capInicial)
{
    this->capacidad = capInicial;
    this->cantidad = 0;
    this->heap = new ElementoTaller[this->capacidad];
}

MinHeapTaller::~MinHeapTaller()
{
    delete[] heap;
}

bool MinHeapTaller::estaVacio() const
{
    return cantidad == 0;
}

int MinHeapTaller::getCantidad() const
{
    return cantidad;
}

void MinHeapTaller::redimensionar(int nuevaCapacidad)
{
    ElementoTaller *nuevoHeap = new ElementoTaller[nuevaCapacidad];
    for (int i = 0; i < cantidad; i++)
    {
        nuevoHeap[i] = heap[i];
    }
    delete[] heap;
    heap = nuevoHeap;
    capacidad = nuevaCapacidad;
}

void MinHeapTaller::flotar(int i)
{
    while (i > 0)
    {
        int padre = (i - 1) / 2;
        if (heap[i].prioridad < heap[padre].prioridad)
        {
            swap(heap[i], heap[padre]);
            i = padre;
        }
        else
        {
            break;
        }
    }
}

void MinHeapTaller::hundir(int i)
{
    while (2 * i + 1 < cantidad)
    {
        int hijoIzq = 2 * i + 1;
        int hijoDer = 2 * i + 2;
        int menor = hijoIzq;

        if (hijoDer < cantidad && heap[hijoDer].prioridad < heap[hijoIzq].prioridad)
        {
            menor = hijoDer;
        }

        if (heap[menor].prioridad < heap[i].prioridad)
        {
            swap(heap[i], heap[menor]);
            i = menor;
        }
        else
        {
            break;
        }
    }
}

// OPERACIÓN 1: Insertar la instancia de Bicicleta
void MinHeapTaller::insertarBicicleta(Bicicleta *bici, int prioridadIngresada)
{
    if (bici == nullptr)
        return;

    if (cantidad == capacidad)
    {
        redimensionar(capacidad * 2);
    }

    ElementoTaller nuevo;
    nuevo.bici = bici;

    // Regla de negocio: si es Eléctrica entra siempre con prioridad 1
    if (bici->getTipo() == "Electrica" || bici->getTipo() == "electrica")
    {
        nuevo.prioridad = 1;
    }
    else
    {
        if (prioridadIngresada < 1)
            nuevo.prioridad = 1;
        else if (prioridadIngresada > 3)
            nuevo.prioridad = 3;
        else
            nuevo.prioridad = prioridadIngresada;
    }

    // Le cambiamos el estado a la bicicleta mediante su setter
    bici->setEstado("En Taller");

    heap[cantidad] = nuevo;
    flotar(cantidad);
    cantidad++;

    cout << "Bicicleta ID " << bici->getId() << " (" << bici->getTipo()
         << ") ingresada al taller con Prioridad " << nuevo.prioridad << ".\n";
}

// OPERACIÓN 2: Extraer la bicicleta más urgente
Bicicleta *MinHeapTaller::extraerMasUrgente()
{
    if (estaVacio())
    {
        cout << "El taller no tiene bicicletas pendientes de reparacion.\n";
        return nullptr;
    }

    Bicicleta *masUrgente = heap[0].bici;

    heap[0] = heap[cantidad - 1];
    cantidad--;

    if (cantidad > 0)
    {
        hundir(0);
    }

    // Al salir del taller, le cambiamos el estado a OK
    masUrgente->setEstado("OK");

    cout << "Bicicleta ID " << masUrgente->getId() << " (" << masUrgente->getTipo()
         << ") reparada.\n";

    return masUrgente;
}

void MinHeapTaller::mostrarTaller() const
{
    if (estaVacio())
    {
        cout << "El taller esta vacio.\n";
        return;
    }

    cout << "\n=== BICICLETAS EN TALLER ===\n";
    for (int i = 0; i < cantidad; i++)
    {
        cout << "ID: " << heap[i].bici->getId()
             << " | Tipo: " << heap[i].bici->getTipo()
             << " | Prioridad: " << heap[i].prioridad << "\n";
    }
}