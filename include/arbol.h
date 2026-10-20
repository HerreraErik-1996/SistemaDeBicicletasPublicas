#pragma once
#include <iostream>
#include <string>
using namespace std;
class Arbol
{
private:
    NodoBici *raiz;

    // Métodos privados auxiliares recursivos
    NodoBici *insertarRec(NodoBici * nodo, const Bicicleta &bici);
    void listarMasDeXKmRec(NodoBici * nodo, int limiteKm, bool &seEncontraron);
    void destruirRec(NodoBici * nodo); // Liberación postorder

public:
    ArbolBicicletas();
    ~ArbolBicicletas();

    void insertar(const Bicicleta &bici);
    void cargarDesdeArchivo(const string &rutaArchivo);

    // Punto B.3: Listar bicicletas con más de X kilómetros
    void listarBicicletasMasDeXKm(int limiteKm);
};