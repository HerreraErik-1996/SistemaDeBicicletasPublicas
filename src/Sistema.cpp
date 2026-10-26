#include "../include/Sistema.h"
#include <fstream>
using namespace std;

Sistema::Sistema()
{
}

Sistema::~Sistema()
{
}

bool Sistema::cargarEstaciones()
{
    ifstream archivo("data/estaciones.txt");

    if (!archivo.is_open())
    {
        cout << "Error al abrir data/estaciones.txt\n";
        return false;
    }

    string encabezado;
    getline(archivo, encabezado);

    string codigo, nombre, barrio;
    int capacidad, disponibles;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> disponibles)
    {
        redEstaciones.agregarEstacion(codigo, nombre, barrio, capacidad, disponibles);
    }

    archivo.close();
    return true;
}

bool Sistema::cargarCiclovias()
{
    ifstream archivo("data/ciclovias.txt");

    if (!archivo.is_open())
    {
        cout << "Error al abrir data/ciclovias.txt\n";
        return false;
    }

    string encabezado;
    getline(archivo, encabezado);

    string origen, destino;

    while (archivo >> origen >> destino)
    {
        redEstaciones.agregarArista(origen, destino);
    }

    archivo.close();
    return true;
}

void Sistema::mostrarMenu() const
{
    cout << "\n===========================================\n";
    cout << "    SISTEMA DE BICICLETAS PUBLICAS    \n";
    cout << "===========================================\n";
    cout << "1. Mostrar Grafo de estaciones / ciclovias\n";
    cout << "2. Listar estaciones ordenadas (Anclajes / Bicis)\n";
    cout << "3. Listar estaciones con menos del 20% de ocupacion\n";
    cout << "0. Salir\n";
    cout << "-------------------------------------------\n";
    cout << "Ingrese una opcion: ";
}

void Sistema::ejecutarListadoOrdenado()
{
    int opcion;
    cout << "\nSeleccione el criterio de ordenamiento:\n";
    cout << "0. Capacidad de anclajes (mayor a menor)\n";
    cout << "1. Bicicletas disponibles (mayor a menor)\n";
    cout << "Opcion: ";
    cin >> opcion;

    if (opcion == 0 || opcion == 1)
    {
        redEstaciones.listarEstacionesOrdenadas(opcion);
    }
    else
    {
        cout << "Opcion no valida.\n";
    }
}

void Sistema::ejecutarListarPorOcupacion()
{
    redEstaciones.listarEstacionesBajaOcupacion();
}

void Sistema::ejecutar()
{
    cout << "Cargando datos del sistema...\n";

    if (!cargarEstaciones() || !cargarCiclovias())
    {
        cout << "Error critico al cargar los archivos. Terminando programa.\n";
        return;
    }

    cout << "Archivos cargados con exito\n";

    int opcion = -1;

    do
    {
        mostrarMenu();

        if (!(cin >> opcion))
        {
            cout << "Entrada invalida. Ingrese un numero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion)
        {
        case 1:
            cout << "\n--- GRAFO DE ESTACIONES Y CICLOVIAS ---\n";
            redEstaciones.mostrar();
            break;
        case 2:
            ejecutarListadoOrdenado();
            break;
        case 3:
            ejecutarListarPorOcupacion();
            break;
        case 0:
            cout << "\nSaliendo del programa...\n";
            break;
        default:
            cout << "\nOpción invalida. Intente de nuevo.\n";
            break;
        }

    } while (opcion != 0);
}