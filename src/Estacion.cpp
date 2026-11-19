#include <string>
#include <iostream>
#include "bicicleta.h"
#include "estacion.h"

using namespace std;

Estacion::Estacion(string codigo, string nombre, string barrio, int capAnclaje, int bicisDisp)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->barrio = barrio;
    this->capAnclaje = capAnclaje;
    this->bicisDisp = bicisDisp;
    this->primeraBicicleta = nullptr;

    // Inicialización de la cola de espera vacía
    this->frenteCola = nullptr;
    this->finCola = nullptr;

}

int Estacion::getCapacidad() const
{
    return capAnclaje;
}

int Estacion::getBicisDisp() const
{
    return bicisDisp;
}

string Estacion::getCodigo() const
{
    return codigo;
}

bool Estacion::agregarBici(Bicicleta *bici)
{
    if (bicisDisp >= capAnclaje || bici == nullptr)
    {
        return false;
    }
    NodoBicicleta *nueva = new NodoBicicleta;
    nueva->bicicleta = bici;
    nueva->siguiente = primeraBicicleta;
    primeraBicicleta = nueva;
    bicisDisp++;
    return true;
}

void Estacion::mostrarInfo() const
{
    cout << "- Nombre: " << nombre
         << " | Barrio: " << barrio
         << " | Capacidad de anclaje: " << capAnclaje
         << " | Bicis Disponibles: " << bicisDisp << "\n";
}

// ---------------------------------------------------------------------
// Manejo Interno de la Cola de Espera
// ---------------------------------------------------------------------

bool Estacion::estaVaciaCola() const
{
    return frenteCola == nullptr;
}

void Estacion::encolarUsuario(const string &dniUsuario)
{
    NodoCola *nuevo = new NodoCola;
    nuevo->dniUsuario = dniUsuario;
    nuevo->siguiente = nullptr;

    if (estaVaciaCola())
    {
        frenteCola = nuevo;
    }
    else
    {
        finCola->siguiente = nuevo;
    }
    finCola = nuevo;
}

string Estacion::desencolarUsuario()
{
    if (estaVaciaCola())
    {
        return "";
    }

    NodoCola *auxiliar = frenteCola;
    string dni = auxiliar->dniUsuario;

    frenteCola = frenteCola->siguiente;
    if (frenteCola == nullptr)
    {
        finCola = nullptr;
    }

    delete auxiliar;
    return dni;
}

// ---------------------------------------------------------------------
// Lógica de Retiro y Devolución con Cola de Espera
// ---------------------------------------------------------------------

Bicicleta* Estacion::solicitarBicicleta(const string &dniUsuario)
{
    // Caso 1: hay bicicletas disponibles
    if (bicisDisp > 0 && primeraBicicleta != nullptr)
    {
        NodoBicicleta *aux = primeraBicicleta;
        Bicicleta *biciRetirada = aux->bicicleta;
        primeraBicicleta = primeraBicicleta->siguiente;
        delete aux; // se retira de la pila local de la estación
        bicisDisp--;

        cout << "Bicicleta asignada con exito al usuario DNI: " << dniUsuario << endl;
        return biciRetirada;
    }

    // Caso 2: No hay bicicletas disponibles, se encola al usuario
    encolarUsuario(dniUsuario);
    cout << "Sin bicis disponibles en " << codigo << ". Usuario DNI " << dniUsuario
        << " ingresado a la cola de espera.\n";
    return nullptr;
}

void Estacion::devolverBicicleta(Bicicleta *bici)
{
    if (bici == nullptr)
        return;

    // si hay gente esperando en la cola, la bicicleta pasa directamente al usuario
    if (!estaVaciaCola())
    {
        string dniSiguiente = desencolarUsuario();
        cout << "Bicicleta devuelta asignada a DNI: " << dniSiguiente
            << " (estaba en cola de espera).\n";
    }
    else
    {
        // si no hay nadie en cola, se ancla en la estación
        if (agregarBici(bici))
        {
            cout << "Bicicleta devuelta y anclada correctamente en estacion " << codigo << ".\n";
        }
        else
        {
            cout << "Error: La estacion " << codigo << " no tiene anclajes libres disponibles.\n";
        }
    }
}

// Destructor
Estacion::~Estacion()
{
    // 1. Liberar memoria de los nodos de bicicletas
    while (primeraBicicleta != nullptr)
    {
        NodoBicicleta *auxiliar = primeraBicicleta;
        primeraBicicleta = primeraBicicleta->siguiente;
        delete auxiliar;
    }

    // 2. Liberar memoria de la cola de espera de usuarios
    while (frenteCola != nullptr)
    {
        NodoCola *auxiliar = frenteCola;
        frenteCola = frenteCola->siguiente;
        delete auxiliar;
    }
    finCola = nullptr;
}