#include "bicicleta.h"
#include "ArbolBicicletas.h"
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
ArbolBicicletas::NodoBici *ArbolBicicletas::insertarRec(NodoBici *nodo, const Bicicleta &bici)
{
    if (nodo == nullptr)
    {
        NodoBici *nuevo = new NodoBici{bici,nullptr,nullptr};
        return nuevo;
    }

    if (bici.getId() < nodo->dato.getId())
    {
        nodo->izquierdo = insertarRec(nodo->izquierdo, bici);
    }
    else if (bici.getId() > nodo->dato.getId())
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

    int biciId;
    string codigoEstacion;
    string tipo;
    string estado;
    int kilometros;
    while (archivo >> biciId >> codigoEstacion >> tipo >> estado >> kilometros)
    {
        Bicicleta b(biciId, codigoEstacion, tipo, estado, kilometros);
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

    if (nodo->dato.kilometraje() > limiteKm){
        cout << "- Bici ID: " << nodo->dato.getId()
        << " | Estacion: " << nodo->dato.getCodigoEstacion()
        << " | Tipo: " << nodo->dato.getTipo()
        << " | Kilometros: " << nodo->dato.kilometraje() << " km\n"
        << " | Estado: " << nodo->dato.getEstado();
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