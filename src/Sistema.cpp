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

string Sistema::uppercase(string *texto)
{
    for (char &letra : *texto)
    {
        letra = toupper(static_cast<unsigned char>(letra));
    }

    return *texto;
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
        Estacion *est = new Estacion(codigo, nombre, barrio, capacidad, 0);
        // 1. Carga en el Grafo (Sección A)
        redEstaciones.agregarEstacion(est);
        // 2. Carga en la Tabla Hash (Sección A)
        tablaEstaciones.insertar(est);
    }
    archivo.close();
    return true;
}

bool Sistema::registrarNuevaEstacion(const string &codigo, const string &nombre, const string &barrio, int capacidad)
{
    // verificamos que no exista otra estacion con el mismo codigo
    if (tablaEstaciones.buscar(codigo) != nullptr)
    {
        cout << "Error: Ya existe una estacion con el codigo '" << codigo << "'.\n";
        return false;
    }

    if (capacidad <= 0)
    {
        cout << "Error: La capacidad debe ser mayor a 0.\n";
        return false;
    }

    // creamos la instancia unica en el heap (disponibles = 0 por defecto)
    Estacion *nuevaEst = new Estacion(codigo, nombre, barrio, capacidad, 0);

    // registramos el mismo puntero en ambas estructuras (propiedad compartida)
    redEstaciones.agregarEstacion(nuevaEst);
    tablaEstaciones.insertar(nuevaEst);

    cout << "Estacion '" << nombre << "' [" << codigo << "] agregada correctamente.\n";

    char respuesta;
    cout << "Desea conectar la nueva estacion con otra? (S/N): ";
    cin >> respuesta;

    while (respuesta == 'S' || respuesta == 's')
    {
      string codigoDestino;

      cout << "Ingrese el codigo de la estacion destino: ";
      cin >> codigoDestino;

      uppercase(&codigoDestino);

      redEstaciones.agregarArista(codigo, codigoDestino);

      cout << "Desea agregar otra conexion? (S/N): ";
      cin >> respuesta;
    }
    return true;
}

bool Sistema::eliminarEstacion(const string &codigo){
    Estacion *est = tablaEstaciones.buscar(codigo);
    if ( est == nullptr){
        cout << "Error: No existe ninguna estacion con el codigo '" << codigo << "'.\n";
        return false;
    }
    //borramos de ambas estructuras(hash y el grafo)
    tablaEstaciones.eliminar(codigo);
    redEstaciones.quitarEstacion(codigo);
    delete est;
    cout << "Estacion '" << codigo << "' y sus ciclovias asociadas eliminadas correctamente.\n";
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
    return listaViajes.cargarDesdeArchivo("data/viajes.txt");
}
void Sistema::registrarNuevoViaje(int idViaje, const string &dniUsuario, const string &codOrigen, const string &codDestino, int fecha, int duracionMin, int kmRecorridos)
{
    Estacion *origen = tablaEstaciones.buscar(codOrigen);
    Estacion *destino = tablaEstaciones.buscar(codDestino);

    if (origen == nullptr || destino == nullptr)
    {
        cout << "Error: Estacion de origen o destino invalida.\n";
        return;
    }

    Bicicleta *bici = origen->solicitarBicicleta(dniUsuario);
    if (bici == nullptr)
    {
        return; // Usuario encolado en espera
    }

    bici->setCodigoEstacion(codDestino);
    bici->agregarKilometros(kmRecorridos);

    destino->devolverBicicleta(bici);

    Viaje nuevoViaje{idViaje, bici->getId(), dniUsuario, codOrigen, codDestino, fecha, duracionMin};
    listaViajes.agregarViaje(nuevoViaje);
}

bool Sistema::cargarBicicletas()
{
    arbolBicis.cargarDesdeArchivo("data/bicicletas.txt");
    arbolBicis.asignarAEstaciones(tablaEstaciones);

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
        cout << "   MODULO A: GESTION DE ESTACIONES Y RED   \n";
        cout << "===========================================\n";
        cout << "1. Mostrar Grafo de estaciones / ciclovias\n";
        cout << "2. Buscar estacion por codigo (Tabla Hash)\n";
        cout << "3. Agregar nueva estacion (Tabla Hash)\n";
        cout << "4. Eliminar estacion (Tabla Hash)\n";
        cout << "5. Listar estaciones ordenadas (Anclajes / Bicis)\n";
        cout << "6. Listar estaciones con menos del 20% de ocupacion\n";
        cout << "7. Calcular camino mas corto por ciclovias (BFS)\n";
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
        {
            string cod;
            cout << "Ingrese el codigo de la estacion a buscar: ";
            cin >> cod;
            Estacion *est = tablaEstaciones.buscar(uppercase(&cod));
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
        
        case 3: {
            string codigo, nombre, barrio;
            int capacidad;
            cout << "Codigo de la nueva estacion: ";
            cin >> codigo;
            cout << "Nombre (sin espacios o con guion_bajo): ";
            cin >> nombre;
            cout << "Barrio: ";
            cin >> barrio;
            cout << "Capacidad de anclajes: ";
            cin >> capacidad;
            registrarNuevaEstacion(codigo, nombre, barrio, capacidad);
            pausar();
            break;
        }

        case 4: {
            string codigo;
            cout << "Codigo de la estacion a borrar: ";
            cin >> codigo;
            eliminarEstacion(codigo);
            pausar();
            break;

        }

        case 5:
        ejecutarListadoOrdenado();
        pausar();
        break;
        
        case 6:
        ejecutarListarPorOcupacion();
        pausar();
        break;

        case 7:
        {
            string origen;
            string destino;

            cout << "Ingrese el codigo de la estacion origen: ";
            cin >> origen;

            cout << "Ingrese el codigo de la estacion destino: ";
            cin >> destino;

            redEstaciones.BFS(origen,destino);
            pausar();
            break;
        }
        case 0:
            break;
        default:
            cout << "Opcion invalida.\n";
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
        cout << "  MODULO B: VIAJES, ESPERA Y TALLER       \n";
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
            ejecutarRetirarDevolverBici();
            pausar();
            break;
        }
        case 5:
        {
            int idBici, prio;
            string tipo;
            cout << "ID de la bicicleta: ";
            cin >> idBici;
            cout << "Prioridad (1-Alta, 2-Media, 3-Baja): ";
            cin >> prio;

            Bicicleta *b = arbolBicis.obtenerBicicleta(idBici);
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
            cout << "Opcion invalida.\n";
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
        cout << "  MODULO C: REGISTRO DE BICICLETAS (ABB)   \n";
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
            cout << "Opcion invalida.\n";
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

void Sistema::ejecutarRetirarDevolverBici(){

  int opcion;
  cout << "1. Retirar bicicleta\n";
  cout << "2. Devolver bicicleta\n";
  cout << "0. Volver atras\n";
  cout << "Ingrese una opcion: ";
  cin >> opcion;
  switch (opcion) {
    case 1:{
      string codEst, dni;
    
      cout << "Codigo de la estacion: ";
      cin >> codEst;
    
      Estacion *est = tablaEstaciones.buscar(uppercase(&codEst));
    
      if (est == nullptr)
      {
        cout << "Estacion no encontrada.\n";
        break;
      }
    
      cout << "DNI del usuario: ";
      cin >> dni;
    
      Bicicleta *bici = est->solicitarBicicleta(dni);
    
      if (bici != nullptr)
      {
        cout << "ID de bicicleta retirada: " << bici->getId() << endl;
      }
    
      break;
    }

    case 2:{
      string codEst;
      int idBici;
  
      cout << "Codigo de la estacion de devolucion: ";
      cin >> codEst;
    
      Estacion *est = tablaEstaciones.buscar(uppercase(&codEst));
    
      if (est == nullptr)
      {
        cout << "Estacion no encontrada.\n";
        break;
      }
    
      cout << "ID de la bicicleta a devolver: ";
      cin >> idBici;
    
      Bicicleta *bici = arbolBicis.obtenerBicicleta(idBici);
    
      if (bici == nullptr)
      {
        cout << "Bicicleta no encontrada.\n";
        break;
      }
    
      est->devolverBicicleta(bici);
      break;
    }
    case 0:
      break;

    default:
      cout << "Opcion invalida.\n";
      break;
  }
}


// ---------------------------------------------------------------------
// Menú Principal
// ---------------------------------------------------------------------

void Sistema::ejecutar()
{
    //limpiarPantalla();

    cargarEstaciones();
    cargarBicicletas();
    cargarCiclovias();
    cargarViajes();

    int opcion = -1;

    do
    {
        //limpiarPantalla();
        cout << "===========================================\n";
        cout << "    SISTEMA DE BICICLETAS PUBLICAS   \n";
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
            cout << "\nOpcion invalida. Intente de nuevo.\n";
            pausar();
            break;
        }

    } while (opcion != 0);
}

// EN LINUX COMPILO CON: g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o programa
