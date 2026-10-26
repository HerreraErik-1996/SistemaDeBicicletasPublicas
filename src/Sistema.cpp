#include "../include/Sistema.h"
#include <fstream>
#include <limits>
#include <cstdlib>

using namespace std;

Sistema::Sistema() {}
Sistema::~Sistema() {}

// ---------------------------------------------------------------------
// Auxiliares de Consola (Limpieza y Pausa)
// ---------------------------------------------------------------------

void Sistema::limpiarPantalla() const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Sistema::pausar() const
{
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ---------------------------------------------------------------------
// Carga de Archivos
// ---------------------------------------------------------------------

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
        // 1. Carga en el Grafo (Sección A)
        redEstaciones.agregarEstacion(codigo, nombre, barrio, capacidad, disponibles);

        // 2. Carga en la Tabla Hash (Sección A)
        Estacion *est = new Estacion(codigo, nombre, barrio, capacidad, disponibles);
        tablaEstaciones.insertar(est);
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

bool Sistema::cargarViajes()
{
    listaViajes.cargarDesdeArchivo("data/viajes.txt");
    return true;
}

bool Sistema::cargarBicicletas()
{
    arbolBicis.cargarDesdeArchivo("data/bicicletas.txt");
    return true;
}

// ---------------------------------------------------------------------
// Submenú 1: Gestión de Estaciones y Red (Módulo A)
// ---------------------------------------------------------------------

void Sistema::menuEstaciones()
{
    int opcion = -1;
    do
    {
        limpiarPantalla();
        cout << "===========================================\n";
        cout << "   MÓDULO A: GESTIÓN DE ESTACIONES Y RED   \n";
        cout << "===========================================\n";
        cout << "1. Mostrar Grafo de estaciones / ciclovias\n";
        cout << "2. Listar estaciones ordenadas (Anclajes / Bicis)\n";
        cout << "3. Listar estaciones con menos del 20% de ocupacion\n";
        cout << "4. Buscar estacion por codigo (Tabla Hash)\n";
        cout << "5. Calcular camino mas corto por ciclovias (BFS)\n";
        cout << "0. Volver al menu principal\n";
        cout << "-------------------------------------------\n";
        cout << "Ingrese una opcion: ";

        if (!(cin >> opcion))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        limpiarPantalla();
        switch (opcion)
        {
        case 1:
            cout << "--- GRAFO DE ESTACIONES Y CICLOVIAS ---\n\n";
            redEstaciones.mostrar();
            pausar();
            break;
        case 2:
            ejecutarListadoOrdenado();
            pausar();
            break;
        case 3:
            ejecutarListarPorOcupacion();
            pausar();
            break;
        case 4:
        {
            string cod;
            cout << "Ingrese el codigo de la estacion a buscar: ";
            cin >> cod;
            Estacion *est = tablaEstaciones.buscar(cod);
            if (est != nullptr)
            {
                cout << "\n--- ESTACION ENCONTRADA EN TABLA HASH ---\n";
                est->mostrarInfo();
            }
            else
            {
                cout << "\nLa estacion " << cod << " no existe en la Tabla Hash.\n";
            }
            pausar();
            break;
        }
        case 5:
            cout << "[En desarrollo] Algoritmo BFS en proceso de implementacion.\n";
            pausar();
            break;
        case 0:
            break;
        default:
            cout << "Opción invalida.\n";
            pausar();
            break;
        }
    } while (opcion != 0);
}

// ---------------------------------------------------------------------
// Submenú 2: Viajes y Colas de Espera / Taller (Módulo B)
// ---------------------------------------------------------------------

void Sistema::menuViajesYColas()
{
    int opcion = -1;
    do
    {
        limpiarPantalla();
        cout << "===========================================\n";
        cout << "  MÓDULO B: VIAJES, ESPERA Y TALLER       \n";
        cout << "===========================================\n";
        cout << "1. Cantidad de viajes por estacion y rango de fechas\n";
        cout << "2. Buscar viajes por DNI de usuario\n";
        cout << "3. Listar bicicletas con mas de X kilometros\n";
        cout << "4. Gestionar Cola de Espera en Estacion (Solicitar/Devolver)\n";
        cout << "5. Ingresar bicicleta al Taller (Min-Heap)\n";
        cout << "6. Extraer bicicleta mas urgente del Taller\n";
        cout << "7. Ver estado del Taller\n";
        cout << "0. Volver al menu principal\n";
        cout << "-------------------------------------------\n";
        cout << "Ingrese una opcion: ";

        if (!(cin >> opcion))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        limpiarPantalla();
        switch (opcion)
        {
        case 1:
        {
            string cod;
            int fIni, fFin;
            cout << "Ingrese codigo de estacion: ";
            cin >> cod;
            cout << "Ingrese fecha inicio (AAAAMMDD): ";
            cin >> fIni;
            cout << "Ingrese fecha fin (AAAAMMDD): ";
            cin >> fFin;

            int total = listaViajes.contarViajesPorEstacionYRango(cod, fIni, fFin);
            cout << "\nTotal de viajes originados en la estacion " << cod << ": " << total << "\n";
            pausar();
            break;
        }
        case 2:
        {
            string dni;
            cout << "Ingrese DNI del usuario: ";
            cin >> dni;
            listaViajes.buscarViajesPorDNI(dni);
            pausar();
            break;
        }
        case 3:
        {
            int km;
            cout << "Ingrese limite de kilometros (X): ";
            cin >> km;
            arbolBicis.listarBicicletasMasDeXKm(km);
            pausar();
            break;
        }
        case 4:
        {
            string codEst, dni;
            cout << "Ingrese el codigo de la estacion: ";
            cin >> codEst;
            Estacion *est = tablaEstaciones.buscar(codEst);

            if (est == nullptr)
            {
                cout << "Estacion no encontrada.\n";
            }
            else
            {
                cout << "Ingrese DNI del usuario: ";
                cin >> dni;
                est->solicitarBicicleta(dni);
            }
            pausar();
            break;
        }
        case 5:
        {
            int idBici, prio;
            string tipo;
            cout << "ID de la bicicleta: ";
            cin >> idBici;
            cout << "Tipo (Comun / Electrica): ";
            cin >> tipo;
            cout << "Prioridad (1-Alta, 2-Media, 3-Baja): ";
            cin >> prio;

            Bicicleta *b = new Bicicleta(idBici, "Taller", tipo, "Averiada", 0);
            tallerReparaciones.insertarBicicleta(b, prio);
            pausar();
            break;
        }
        case 6:
            tallerReparaciones.extraerMasUrgente();
            pausar();
            break;
        case 7:
            tallerReparaciones.mostrarTaller();
            pausar();
            break;
        case 0:
            break;
        default:
            cout << "Opción invalida.\n";
            pausar();
            break;
        }
    } while (opcion != 0);
}

// ---------------------------------------------------------------------
// Submenú 3: Registro ABB de Bicicletas (Módulo C)
// ---------------------------------------------------------------------

void Sistema::menuRegistroBicicletas()
{
    int opcion = -1;
    do
    {
        limpiarPantalla();
        cout << "===========================================\n";
        cout << "  MÓDULO C: REGISTRO DE BICICLETAS (ABB)   \n";
        cout << "===========================================\n";
        cout << "1. Listar todas las bicicletas (Inorder)\n";
        cout << "2. Buscar bicicleta por ID\n";
        cout << "3. Contar bicicletas averiadas\n";
        cout << "4. Ver altura del arbol de bicicletas\n";
        cout << "0. Volver al menu principal\n";
        cout << "-------------------------------------------\n";
        cout << "Ingrese una opcion: ";

        if (!(cin >> opcion))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        limpiarPantalla();
        switch (opcion)
        {
        case 1:
            arbolBicis.listarInorder();
            pausar();
            break;
        case 2:
        {
            int idBici;
            cout << "Ingrese ID de la bicicleta: ";
            cin >> idBici;
            arbolBicis.buscarPorId(idBici);
            pausar();
            break;
        }
        case 3:
            cout << "\nCantidad de bicicletas averiadas: " << arbolBicis.contarAveriadas() << "\n";
            pausar();
            break;
        case 4:
            cout << "\nAltura del ABB de bicicletas: " << arbolBicis.calcularAltura() << "\n";
            pausar();
            break;
        case 0:
            break;
        default:
            cout << "Opción invalida.\n";
            pausar();
            break;
        }
    } while (opcion != 0);
}

// ---------------------------------------------------------------------
// Acciones Auxiliares
// ---------------------------------------------------------------------

void Sistema::ejecutarListadoOrdenado()
{
    int opcion;
    cout << "Seleccione el criterio de ordenamiento:\n";
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

// ---------------------------------------------------------------------
// Menú Principal
// ---------------------------------------------------------------------

void Sistema::ejecutar()
{
    limpiarPantalla();
    cout << "Cargando datos del sistema...\n";

    cargarEstaciones();
    cargarCiclovias();
    cargarViajes();
    cargarBicicletas();

    cout << "Archivos cargados con exito.\n";
    pausar();

    int opcion = -1;

    do
    {
        limpiarPantalla();
        cout << "===========================================\n";
        cout << "    SISTEMA DE BICICLETAS PÚBLICAS (C++)   \n";
        cout << "===========================================\n";
        cout << "1. Gestion de Estaciones y Red (Modulo A)\n";
        cout << "2. Viajes, Espera y Taller (Modulo B)\n";
        cout << "3. Registro de Bicicletas - ABB (Modulo C)\n";
        cout << "0. Salir del Sistema\n";
        cout << "-------------------------------------------\n";
        cout << "Ingrese una opcion: ";

        if (!(cin >> opcion))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcion)
        {
        case 1:
            menuEstaciones();
            break;
        case 2:
            menuViajesYColas();
            break;
        case 3:
            menuRegistroBicicletas();
            break;
        case 0:
            limpiarPantalla();
            cout << "\nSaliendo del programa...\n";
            break;
        default:
            cout << "\nOpción invalida. Intente de nuevo.\n";
            pausar();
            break;
        }

    } while (opcion != 0);
}

// EN LINUX COMPILO CON: g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o programa
