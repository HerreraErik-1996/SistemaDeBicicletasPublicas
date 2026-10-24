#pragma once
#include <string>
#include <iostream>
#include "bicicleta.h"

class Estacion
{
private:
    struct NodoBicicleta
    {
        Bicicleta *bicicleta;
        NodoBicicleta *siguiente;
    };

    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capAnclaje;
    int bicisDisp;
    NodoBicicleta *primeraBicicleta;

public:
    Estacion(std::string cod, std::string nom, std::string bar, int capAn, int bcDisp);

    int getCapacidad() const;
    int getBicisDisp() const;

    bool agregarBici(Bicicleta *bici);
    void mostrarInfo() const;
    string getCodigo() const;
    ~Estacion();
    Estacion &operator=(const Estacion &) = delete;
};