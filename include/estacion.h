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
    Estacion(string codigo, string nombre, string barrio, int capAnclaje, int bicisDisp){
        codigo;
        nombre;
        barrio;
        capAnclaje;
        bicisDisp;
        primeraBicicleta = nullptr;
    }

    int getCapacidad(){
        return capAnclaje;
    }

    int getBicisDisp(){
        return bicisDisp;
    }
    
    bool agregarBici(Bicicleta *bici){
        if (bicisDisp >= capAnclaje || bici == nullptr){
            return false;
        }
        NodoBicicleta* nueva = new NodoBicicleta;
        nueva->bicicleta=bici;
        nueva->siguiente= primeraBicicleta;
        primeraBicicleta=nueva;

        bicisDisp++;
        return true;
    }

    void mostrarInfo(){
        cout << "- Nombre: " << nombre 
        << " | Estacion: " << barrio
        << " | Tipo: " << capAnclaje
        << " | Kilometros: " << bicisDisp;
    }

    ~Estacion()
    {
        while (primeraBicicleta != nullptr)
        {
            NodoBicicleta* auxiliar = primeraBicicleta;
            primeraBicicleta = primeraBicicleta->siguiente;
            delete auxiliar;
        }
    }
    Estacion(const Estacion&) = delete;
    Estacion& operator=(const Estacion&) = delete;
};