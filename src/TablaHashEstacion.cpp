#include "../include/TablaHashEstacion.h"

int TablaHashEstaciones::funcionHash(const std::string &codigo) const
{
    unsigned long hash = 5381;
    for (char c : codigo)
    {
        hash = ((hash << 5) + hash) + c; // hash * 32 + c
    }
    return static_cast<int>(hash % capacidad);
}

TablaHashEstaciones::TablaHashEstaciones(int cap)
{
    capacidad = cap;
    cantidad = 0;
    colisiones = 0;

    tabla = new NodoHash *[capacidad];
    for (int i = 0; i < capacidad; ++i)
    {
        tabla[i] = nullptr;
    }
}

TablaHashEstaciones::~TablaHashEstaciones()
{
    for (int i = 0; i < capacidad; ++i)
    {
        NodoHash *actual = tabla[i];
        while (actual != nullptr)
        {
            NodoHash *aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar->estacion;
            delete aBorrar;
        }
    }
    delete[] tabla;
}

bool TablaHashEstaciones::insertar(Estacion *estacion)
{
    int indice = funcionHash(estacion->getCodigo());

    if (tabla[indice] != nullptr)
    {
        colisiones++;
    }
    NodoHash *actual = tabla[indice];

    while (actual != nullptr)
    {
        if (actual->estacion->getCodigo() == estacion->getCodigo())
        {
            return false; // Código duplicado, no se inserta
        }
        actual = actual->siguiente;
    }

    NodoHash *nuevoNodo = new NodoHash(estacion);
    nuevoNodo->siguiente = tabla[indice];
    tabla[indice] = nuevoNodo;
    cantidad++;

    return true;
}

Estacion *TablaHashEstaciones::buscar(const std::string &codigo)
{
    int indice = funcionHash(codigo);
    NodoHash *actual = tabla[indice];

    while (actual != nullptr)
    {
        if (actual->estacion->getCodigo() == codigo)
        {
            return actual->estacion;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool TablaHashEstaciones::eliminar(const std::string &codigo)
{
    int indice = funcionHash(codigo);
    NodoHash *actual = tabla[indice];
    NodoHash *anterior = nullptr;

    while (actual != nullptr)
    {
        if (actual->estacion->getCodigo() == codigo)
        {
            if (anterior == nullptr)
            {
                tabla[indice] = actual->siguiente;
            }
            else
            {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            cantidad--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

double TablaHashEstaciones::obtenerFactorDeCarga() const
{
    return static_cast<double>(cantidad) / capacidad;
}

int TablaHashEstaciones::obtenerColisiones() const
{
    return colisiones;
}

int TablaHashEstaciones::obtenerCantidad() const
{
    return cantidad;
}

void TablaHashEstaciones::mostrarColisiones() const
{
    std::cout << "\n- Estadisticas de la Tabla Hash -\n";
    std::cout << "Estaciones cargadas: " << cantidad << " / " << capacidad << "\n";
    std::cout << "Factor de carga: " << obtenerFactorDeCarga() << "\n";

    if (colisiones == 0)
    {
        std::cout << "Colisiones registradas: 0 (no hubo colisiones durante la carga).\n";
    }
    else
    {
        std::cout << "Colisiones registradas: " << colisiones << "\n";
    }
}