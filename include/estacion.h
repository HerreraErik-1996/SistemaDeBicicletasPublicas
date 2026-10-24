#pragma once
#include <string>
#include <iostream>
#include "bicicleta.h"

using namespace std;

class Estacion
{
private:
    struct NodoBicicleta
    {
        Bicicleta *bicicleta;
        NodoBicicleta *siguiente;
    };

    string codigo;
    string nombre;
    string barrio;
    int capAnclaje;
    int bicisDisp;
    NodoBicicleta *primeraBicicleta;

public:
    Estacion(string cod, string nom, string bar, int capAn, int bcDisp)
    {
        codigo = cod;
        nombre = nom;
        barrio = bar;
        capAnclaje = capAn;
        bicisDisp = bcDisp;
        primeraBicicleta = nullptr;
    }

    int getCapacidad() const
    {
        return capAnclaje;
    }

    int getBicisDisp() const
    {
        return bicisDisp;
    }

    bool agregarBici(Bicicleta *bici)
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

    void mostrarInfo() const
    {
        cout << "- Nombre: " << nombre
             << " | Estacion: " << barrio
             << " | Tipo: " << capAnclaje
             << " | Kilometros: " << bicisDisp;
    }

    string getCodigo() const
    {
        return codigo;
    }

    ~Estacion()
    {
        while (primeraBicicleta != nullptr)
        {
            NodoBicicleta *auxiliar = primeraBicicleta;
            primeraBicicleta = primeraBicicleta->siguiente;
            delete auxiliar;
        }
    }
    Estacion &operator=(const Estacion &) = delete;
};