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

    // Estructura para los nodos de la Cola de Espera (guardamos DNI del usuario)
    struct NodoCola
    {
        std::string dniUsuario;
        NodoCola *siguiente;
    };

    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capAnclaje;
    int bicisDisp;

    NodoBicicleta *primeraBicicleta;

    // Atributos privados para administrar la Cola de Espera
    NodoCola *frenteCola;
    NodoCola *finCola;

public:
    Estacion(std::string cod, std::string nom, std::string bar, int capAn, int bcDisp);

    int getCapacidad() const;
    int getBicisDisp() const;
    std::string getCodigo() const;

    bool agregarBici(Bicicleta *bici);
    void mostrarInfo() const;

    // Métodos para la Cola de Espera
    void encolarUsuario(const std::string &dniUsuario);
    std::string desencolarUsuario();
    bool estaVaciaCola() const;

    // Operaciones del sistema
    Bicicleta* solicitarBicicleta(const std::string &dniUsuario);
    void devolverBicicleta(Bicicleta *bici);

    ~Estacion();
    Estacion &operator=(const Estacion &) = delete;
};