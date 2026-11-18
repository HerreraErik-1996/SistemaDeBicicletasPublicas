#pragma once
#include <string>
using namespace std;

// Estructura pura de datos para un registro de viaje
struct Viaje
{
    int viajeId;
    int biciId;
    string dniUsuario;
    string estacionOrigen;
    string estacionDestino;
    int fecha; // Formato AAAAMMDD
    int duracionMin;
};

// Nodo de la lista enlazada
struct NodoViaje
{
    Viaje datos;
    NodoViaje *siguiente;
};

// Clase TDA para administrar los viajes
class ListaViajes
{
private:
    NodoViaje *cabeza; // Puntero al primer nodo
    int cantidad;      // Cantidad total de elementos

public:
    ListaViajes();
    ~ListaViajes(); // Destructor para liberar memoria con delete nodo por nodo

    // Carga de datos
    void agregarViaje(const Viaje &nuevoViaje);
    bool cargarDesdeArchivo(const string &rutaArchivo);

    // Funcionalidades asignadas (B.1 y B.2)
    int contarViajesPorEstacionYRango(const string &codigoEstacion, int fechaInicio, int fechaFin);
    void buscarViajesPorDNI(const string &dniBuscado);
    void listarBicicletasConMasDe_Km(int km);
};