#pragma once
#include <string>
using namespace std;
#include "Bicicleta.h"

class Bicicleta
{
private:
    int id;
    std::string codigoEstacion;
    std::string tipo;   // "Mecanica" o "Electrica"
    std::string estado; // "OK" o "Averiada"
    int kilometros;

public:
    Bicicleta::Bicicleta(int biciId, const string& codigoEstacion, const string& tipo, const string& estado, int kilometros)
    {
        this->id = biciId;
        this->codigoEstacion = codigoEstacion;
        this->tipo = tipo;
        this->estado = estado;
        this->kilometros = kilometros;
    }

    int Bicicleta::getId(){
        return id;
    }
    string Bicicleta::getCodigoEstacion(){
        return codigoEstacion;
    }
    string Bicicleta::getTipo(){
        return tipo;
    }
    string Bicicleta::getEstado(){
        return estado;
    }

    bool Bicicleta::estaDisponible(){
        return estado == "OK";
    }

    int Bicicleta::kilometraje(){
        return kilometros;
    }

    void Bicicleta::setEstado(const string& nuevoEstado){
        estado = nuevoEstado;
    }
    void Bicicleta::setCodigoEstacion(const string& nuevoCodigo){
        codigoEstacion = nuevoCodigo;
    }
    void Bicicleta::agregarKilometros(int km){
        kilometros += km;
    }
};