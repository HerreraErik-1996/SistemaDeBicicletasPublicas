#pragma once
#include <string>

using namespace std;

class Bicicleta
{
private:
    int biciId;
    string codigoEstacion;
    string tipo;   // "Mecanica" o "Electrica"
    string estado; // "OK" o "Averiada"
    int kilometros;

public:
    Bicicleta(int biciId,string codigoEstacion, string tipo, string estado, int kilometros){
        biciId;
        codigoEstacion;
        tipo;
        estado;
        kilometros;
    }

    int getCodigo(){
        return biciId;
    }

    bool estaDisponible(){
        return estado == "OK" ;
    }

    int kilometraje(){
        return kilometros ;
    }
};