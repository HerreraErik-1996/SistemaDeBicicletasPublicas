#include <string>
#include <iostream>
#include "Bicicleta.h"
#include "Estacion.h"
using namespace std;

Estacion::Estacion(string codigo, string nombre, string barrio, int capAnclaje, int bicisDisp)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->barrio = barrio;
    this->capAnclaje = capAnclaje;
    this->bicisDisp = bicisDisp;
    this->primeraBicicleta = nullptr;
}

int Estacion::getCapacidad() const
{
    return capAnclaje;
}
int Estacion::getBicisDisp() const
{
    return bicisDisp;
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
         << " | Barrio: " << nombre
         << " | Capacidad de anclaje: " << capAnclaje
         << " | Bicis Disponibles: " << bicisDisp;
}
string Estacion::getCodigo() const
{
    return codigo;
}
Estacion::~Estacion()
{
    while (primeraBicicleta != nullptr)
    {
        NodoBicicleta *auxiliar = primeraBicicleta;
        primeraBicicleta = primeraBicicleta->siguiente;
        delete auxiliar;
    }
}