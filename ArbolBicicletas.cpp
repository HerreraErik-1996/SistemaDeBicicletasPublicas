#include "bicicleta.h"
#include <iostream>
#include <fstream>
using namespace std;

ArbolBicicletas::ArbolBicicletas()
{
    raiz = nullptr;
}

// Destructor con recorrido postorder (liberación estricta exigida por consigna)
ArbolBicicletas::~ArbolBicicletas()
{
    destruirRec(raiz);
}

void ArbolBicicletas::destruirRec(NodoBici *nodo)
{
    if (nodo != nullptr)
    {
        destruirRec(nodo->izquierdo);
        destruirRec(nodo->derecho);
        delete nodo; // Libera nodo por nodo
    }
}

// Inserción en el Árbol Binario de Búsqueda ordenado por biciId
NodoBici *ArbolBicicletas::insertarRec(NodoBici *nodo, const Bicicleta &bici)
{
    if (nodo == nullptr)
    {
        NodoBici *nuevo = new NodoBici;
        nuevo->datos = bici;
        nuevo->izquierdo = nullptr;
        nuevo->derecho = nullptr;
        return nuevo;
    }

    if (bici.biciId < nodo->datos.biciId)
    {
        nodo->izquierdo = insertarRec(nodo->izquierdo, bici);
    }
    else if (bici.biciId > nodo->datos.biciId)
    {
        nodo->derecho = insertarRec(nodo->derecho, bici);
    }

    return nodo;
}

void ArbolBicicletas::insertar(const Bicicleta &bici)
{
    raiz = insertarRec(raiz, bici);
}

// Lectura de bicicletas.txt
void ArbolBicicletas::cargarDesdeArchivo(const string &rutaArchivo)
{
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo " << rutaArchivo << endl;
        return;
    }

    Bicicleta b;
    while (archivo >> b.biciId >> b.codigoEstacion >> b.tipo >> b.estado >> b.kilometros)
    {
        insertar(b);
    }

    archivo.close();
}

// Función auxiliar para recorrer todo el árbol y verificar el kilometraje
void ArbolBicicletas::listarMasDeXKmRec(NodoBici *nodo, int limiteKm, bool &seEncontraron)
{
    if (nodo == nullptr)
        return;

    // Recorrido inorder (visita subárbol izq, nodo actual, subárbol der)
    listarMasDeXKmRec(nodo->izquierdo, limiteKm, seEncontraron);

    if (nodo->datos.kilometros > limiteKm)
    {
        cout << "- Bici ID: " << nodo->datos.biciId
             << " | Estacion: " << nodo->datos.codigoEstacion
             << " | Tipo: " << nodo->datos.tipo
             << " | Estado: " << nodo->datos.estado
             << " | Kilometros: " << nodo->datos.kilometros << " km\n";
        seEncontraron = true;
    }

    listarMasDeXKmRec(nodo->derecho, limiteKm, seEncontraron);
}

// Punto B.3: Método público que llama el menú
void ArbolBicicletas::listarBicicletasMasDeXKm(int limiteKm)
{
    bool seEncontraron = false;

    cout << "\n--- BICICLETAS CON MÁS DE " << limiteKm << " KM (PARA REVISIÓN) ---\n";
    listarMasDeXKmRec(raiz, limiteKm, seEncontraron);

    if (!seEncontraron)
    {
        cout << "No se encontraron bicicletas que superen los " << limiteKm << " km.\n";
    }
}