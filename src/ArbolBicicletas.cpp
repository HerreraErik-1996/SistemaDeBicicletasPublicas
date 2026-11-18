#include "../include/ArbolBicicletas.h"
#include <fstream>

using namespace std;

ArbolBicicletas::ArbolBicicletas()
{
    raiz = nullptr;
}

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
        delete nodo;
    }
}

// 1. Insertar en ABB
ArbolBicicletas::NodoBici *ArbolBicicletas::insertarRec(NodoBici *nodo, const Bicicleta &bici)
{
    if (nodo == nullptr)
    {
        return new NodoBici{bici, nullptr, nullptr};
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

// 2. Buscar por ID y mostrar datos
ArbolBicicletas::NodoBici *ArbolBicicletas::buscarRec(NodoBici *nodo, int id) const
{
    if (nodo == nullptr || nodo->dato.getId() == id)
    {
        return nodo;
    }

    if (id < nodo->dato.getId())
    {
        return buscarRec(nodo->izquierdo, id);
    }
    return buscarRec(nodo->derecho, id);
}

bool ArbolBicicletas::buscarPorId(int id) const
{
    NodoBici *resultado = buscarRec(raiz, id);
    if (resultado != nullptr)
    {
        cout << "\n--- BICICLETA ENCONTRADA ---\n";
        cout << "ID: " << resultado->dato.getId()
             << " | Estacion: " << resultado->dato.getCodigoEstacion()
             << " | Tipo: " << resultado->dato.getTipo()
             << " | Estado: " << resultado->dato.getEstado()
             << " | KM: " << resultado->dato.kilometraje() << "\n";
        return true;
    }
    cout << "\nBicicleta con ID " << id << " no encontrada en el sistema.\n";
    return false;
}

// 3. Listar todas las bicicletas ordenadas por ID (Inorder)
void ArbolBicicletas::inorderRec(NodoBici *nodo) const
{
    if (nodo == nullptr)
        return;

    inorderRec(nodo->izquierdo);
    cout << "ID: " << nodo->dato.getId()
         << " | Estacion: " << nodo->dato.getCodigoEstacion()
         << " | Tipo: " << nodo->dato.getTipo()
         << " | Estado: " << nodo->dato.getEstado()
         << " | KM: " << nodo->dato.kilometraje() << "\n";
    inorderRec(nodo->derecho);
}

void ArbolBicicletas::listarInorder() const
{
    if (raiz == nullptr)
    {
        cout << "El arbol de bicicletas esta vacio.\n";
        return;
    }
    cout << "\n--- LISTADO DE BICICLETAS (INORDER) ---\n";
    inorderRec(raiz);
}

// 4. Contar cuantas bicicletas averiadas hay
int ArbolBicicletas::contarAveriadasRec(NodoBici *nodo) const
{
    if (nodo == nullptr)
        return 0;

    int averiadaActual = (nodo->dato.getEstado() == "Averiada" || nodo->dato.getEstado() == "averiada") ? 1 : 0;

    return averiadaActual + contarAveriadasRec(nodo->izquierdo) + contarAveriadasRec(nodo->derecho);
}

int ArbolBicicletas::contarAveriadas() const
{
    return contarAveriadasRec(raiz);
}

// 5. Calcular la altura del árbol
int ArbolBicicletas::calcularAlturaRec(NodoBici *nodo) const
{
    if (nodo == nullptr)
        return 0; // O -1 si consideras altura por aristas en lugar de nodos

    int altIzq = calcularAlturaRec(nodo->izquierdo);
    int altDer = calcularAlturaRec(nodo->derecho);

    return 1 + (altIzq > altDer ? altIzq : altDer);
}

int ArbolBicicletas::calcularAltura() const
{
    return calcularAlturaRec(raiz);
}

// Carga desde archivo
void ArbolBicicletas::cargarDesdeArchivo(const string &rutaArchivo)
{
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo " << rutaArchivo << endl;
        return;
    }

    int biciId;
    string codigoEstacion, tipo, estado;
    int kilometros;
    while (archivo >> biciId >> codigoEstacion >> tipo >> estado >> kilometros)
    {
        Bicicleta b(biciId, codigoEstacion, tipo, estado, kilometros);
        insertar(b);
    }

    archivo.close();
}

// B.3 Filtro por Kilómetros
void ArbolBicicletas::listarMasDeXKmRec(NodoBici *nodo, int limiteKm, bool &seEncontraron) const
{
    if (nodo == nullptr)
        return;

    listarMasDeXKmRec(nodo->izquierdo, limiteKm, seEncontraron);

    if (nodo->dato.kilometraje() > limiteKm)
    {
        cout << "- Bici ID: " << nodo->dato.getId()
             << " | Estacion: " << nodo->dato.getCodigoEstacion()
             << " | Tipo: " << nodo->dato.getTipo()
             << " | Kilometros: " << nodo->dato.kilometraje() << " km"
             << " | Estado: " << nodo->dato.getEstado() << "\n";
        seEncontraron = true;
    }

    listarMasDeXKmRec(nodo->derecho, limiteKm, seEncontraron);
}

void ArbolBicicletas::listarBicicletasMasDeXKm(int limiteKm) const
{
    bool seEncontraron = false;
    cout << "\n--- BICICLETAS CON MÁS DE " << limiteKm << " KM ---\n";
    listarMasDeXKmRec(raiz, limiteKm, seEncontraron);

    if (!seEncontraron)
    {
        cout << "No se encontraron bicicletas que superen los " << limiteKm << " km.\n";
    }
}

Bicicleta *ArbolBicicletas::obtenerBicicleta(int idBici)
{
    NodoBici *nodoEncontrado = buscarRec(raiz, idBici);

    if (nodoEncontrado != nullptr)
    {
        return &(nodoEncontrado->dato); // Devuelve la dirección de memoria de la instancia real
    }

    return nullptr; // Retorna nullptr si el ID no existe en el árbol
}

void ArbolBicicletas::asignarAEstacionesRec(NodoBici *nodo, TablaHashEstaciones &tabla)
{
    if (nodo == nullptr)
        return;

    asignarAEstacionesRec(nodo->izquierdo, tabla);

    // 1. Buscamos la estación en la Tabla Hash usando el código que tiene la bici
    Estacion *est = tabla.buscar(nodo->dato.getCodigoEstacion()); // Ajusta el nombre del método buscar si difiere

    // 2. Si la estación existe, agregamos el puntero de la bicicleta a su lista enlazada
    if (est != nullptr)
    {
        est->agregarBici(&(nodo->dato));
    }

    asignarAEstacionesRec(nodo->derecho, tabla);
}

void ArbolBicicletas::asignarAEstaciones(TablaHashEstaciones &tabla)
{
    asignarAEstacionesRec(raiz, tabla);
}

// Punto.C(Opcional)
ArbolBicicletas::NodoBici *ArbolBicicletas::eliminarRec(
    NodoBici *nodo, int id, bool &eliminado)
{
    // no se encontro el nodo
    if (nodo == nullptr)
        return nullptr;

    // buscar por izquierda
    if (id < nodo->dato.getId())
    {
        nodo->izquierdo = eliminarRec(
            nodo->izquierdo, id, eliminado);
    }

    // buscar por derecha
    else if (id > nodo->dato.getId())
    {
        nodo->derecho = eliminarRec(
            nodo->derecho, id, eliminado);
    }

    // encontramos el nodo a eliminar
    else
    {
        eliminado = true;

        // CASO 1 y 2: sin hijo izquierdo
        if (nodo->izquierdo == nullptr)
        {
            NodoBici *derecho = nodo->derecho;
            delete nodo;
            return derecho;
        }

        // CASO 2: sin hijo derecho
        if (nodo->derecho == nullptr)
        {
            NodoBici *izquierdo = nodo->izquierdo;
            delete nodo;
            return izquierdo;
        }

        // CASO 3: tiene ambos hijos
        // buscamos el menor del subarbol derecho
        NodoBici **enlaceSucesor = &nodo->derecho;

        while ((*enlaceSucesor)->izquierdo != nullptr)
        {
            enlaceSucesor = &((*enlaceSucesor)->izquierdo);
        }

        NodoBici *sucesor = *enlaceSucesor;

        // desvinculamos el sucesor de su posicion original
        *enlaceSucesor = sucesor->derecho;

        // reubicamos el sucesor
        sucesor->izquierdo = nodo->izquierdo;
        sucesor->derecho = nodo->derecho;

        delete nodo;

        return sucesor;
    }

    return nodo;
}

bool ArbolBicicletas::eliminar(int id)
{
    bool eliminado = false;

    raiz = eliminarRec(raiz, id, eliminado);

    return eliminado;
}