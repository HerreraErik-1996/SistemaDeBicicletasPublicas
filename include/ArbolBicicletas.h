#pragma once
#include <iostream>
#include <string>
#include <algorithm> // Para std::max en la altura
#include "bicicleta.h"
#include "TablaHashEstacion.h"

using namespace std;

class ArbolBicicletas
{
private:
    struct NodoBici
    {
        Bicicleta dato;
        NodoBici *izquierdo;
        NodoBici *derecho;
    };

    NodoBici *raiz;

    // Métodos auxiliares recursivos
    NodoBici *insertarRec(NodoBici *nodo, const Bicicleta &bici);
    NodoBici *buscarRec(NodoBici *nodo, int id) const;
    void inorderRec(NodoBici *nodo) const;
    int contarAveriadasRec(NodoBici *nodo) const;
    int calcularAlturaRec(NodoBici *nodo) const;
    void listarMasDeXKmRec(NodoBici *nodo, int limiteKm, bool &seEncontraron) const;
    void destruirRec(NodoBici *nodo); // Postorder
    void asignarAEstacionesRec(NodoBici *nodo, TablaHashEstaciones &tabla);

public:
    ArbolBicicletas();
    ~ArbolBicicletas();

    void cargarDesdeArchivo(const string &rutaArchivo);
    void insertar(const Bicicleta &bici);
    void asignarAEstaciones(TablaHashEstaciones &tabla);

    // Métodos requeridos por la consigna C
    bool buscarPorId(int id) const;
    void listarInorder() const;
    int contarAveriadas() const;
    int calcularAltura() const;


    // Punto B.3 previo
    void listarBicicletasMasDeXKm(int limiteKm) const;
    // Punto B.5
    Bicicleta *obtenerBicicleta(int idBici);
};