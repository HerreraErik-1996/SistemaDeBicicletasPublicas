#include <string>
#include <iostream>
#include "Grafo.h"
#include "Cola.h"

using namespace std;

Grafo::Grafo()
{
    cantidadVertices = 0;
    capacidadVertices = 0;
    vertices = nullptr;
}
int Grafo::buscarId(const string &codigo)
{
    for (int i = 0; i < cantidadVertices; i++)
    {
        if (vertices[i].estacion->getCodigo() == codigo)
        {
            return i;
        }
    }
    return -1;
}
Grafo::~Grafo()
{
    if (vertices != nullptr)
    {
        // liberamos los nodos de la lista de adyacencia de cada vertice
        for (int i = 0; i < cantidadVertices; i++)
        {
            Nodo *actual = vertices[i].adyacentes;
            while (actual != nullptr)
            {
                Nodo *aux = actual;
                actual = actual->siguiente;
                delete aux;
            }
            // NO hacemos delete vertices[i].estacion; porque su dueño es la Tabla Hash
        }

        // 2. Liberamos el arreglo dinámico de vértices
        delete[] vertices;
        vertices = nullptr;
    }
}

void Grafo::agregarEstacion(Estacion* est)
{
    if (est == nullptr){
        return;
    }
    if (buscarId(est->getCodigo()) != -1)
    {
        cout << "La estacion " << est->getCodigo() << " ya existe" << endl;
        return;
    }
    if (cantidadVertices == capacidadVertices)
    {
        int nuevaCap;

        if (capacidadVertices == 0)
        {
            nuevaCap = 2;
        }
        else
        {
            nuevaCap = capacidadVertices * 2;
        }
        Vertice *nuevosVertices = new Vertice[nuevaCap];

        for (int i = 0; i < cantidadVertices; i++)
        {
            nuevosVertices[i] = vertices[i];
        }
        delete[] vertices;
        vertices = nuevosVertices;
        capacidadVertices = nuevaCap;
    }
    
    vertices[cantidadVertices].estacion = est;
    vertices[cantidadVertices].adyacentes = nullptr;
    cantidadVertices++;
}

bool Grafo::agregarArista(const string &origen, const string &destino)
{
    int idOrigen = buscarId(origen);
    int idDestino = buscarId(destino);
    // verifica que las estaciones existan
    if (idOrigen == -1 || idDestino == -1)
    {
        cout << "Error: alguna de las estaciones no existe." << endl;
        return false;
    }
    // evita bucle
    if (idOrigen == idDestino)
    {
      return false;
    }
    // evitar aristas duplicadas
    Nodo *actual = vertices[idOrigen].adyacentes;
    while (actual != nullptr)
    {
        if (actual->idVertice == idDestino)
        {
            return false;
        }
        actual = actual->siguiente;
    }

    Nodo *nuevo = new Nodo;
    nuevo->idVertice = idDestino;
    nuevo->siguiente = vertices[idOrigen].adyacentes;
    vertices[idOrigen].adyacentes = nuevo;

    // porque es grafo no dirigido
    Nodo *nuevo2 = new Nodo;
    nuevo2->idVertice = idOrigen;
    nuevo2->siguiente = vertices[idDestino].adyacentes;
    vertices[idDestino].adyacentes = nuevo2;

    return true;
};

void Grafo::mostrar()
{
    for (int i = 0; i < cantidadVertices; i++)
    {
        cout << "Estacion " << vertices[i].estacion->getCodigo() << ": ";

        Nodo *actual = vertices[i].adyacentes;

        while (actual != nullptr)
        {
            int id = actual->idVertice;
            cout << vertices[id].estacion->getCodigo();
            actual = actual->siguiente;
            if (actual != nullptr)
            {
                cout << " -> ";
            }
        }
        cout << endl;
    }
};

bool Grafo::quitarEstacion(const string &codigo){
    int id = buscarId(codigo);
    if (id == -1){
        return false;
    }
    Nodo *actual = vertices[id].adyacentes;
    while (actual != nullptr)
    {
        Nodo *temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    vertices[id].adyacentes = nullptr;
    //recorremos las demas estaciones para eliminar aristas entrantes hacia id y actualizar los indices del arreglo
    for (int i = 0; i < cantidadVertices; i++){
        if(i == id){
            continue;
        }
        Nodo *actual = vertices[i].adyacentes;
        Nodo *anterior = nullptr;
        while (actual != nullptr){
            if (actual->idVertice == id){
                //borramos la arista que apunta a el id de la estacion eliminada
                Nodo *aBorrar = actual;
                if (anterior == nullptr){
                    vertices[i].adyacentes = actual->siguiente;
                }else{
                    anterior->siguiente = actual->siguiente;
                }
                actual = actual->siguiente;
                delete aBorrar;
            }else{
                //si apuntaba a una estacion posterior a la borrada, el indice baja en 1
                if (actual->idVertice > id){
                    actual->idVertice--;
                }
                anterior = actual;
                actual = actual->siguiente;
            }
        }
    }
    // compactamos el arreglo vertices y actualizamos id
    for (int i = id; i < cantidadVertices-1; i++){
        vertices[i] = vertices[i +1];
        vertices[i].id = i;
    }
    //limpiamos el final
    vertices[cantidadVertices - 1].estacion = nullptr;
    vertices[cantidadVertices - 1].adyacentes = nullptr;
    cantidadVertices --;

    return true;
}

void Grafo::listarEstacionesOrdenadas(int tipoListado)
{
    if (this->vertices == nullptr || this->cantidadVertices <= 0)
    {
        cout << "No hay estaciones registradas en el grafo.\n";
        return;
    }

    // 1. Crear copia auxiliar extrayendo el puntero 'estacion' de cada Vertice
    Estacion **listaOrdenada = new Estacion *[this->cantidadVertices];
    for (int i = 0; i < this->cantidadVertices; i++)
    {
        listaOrdenada[i] = this->vertices[i].estacion;
    }

    // 2. Ordenamiento por Inserción (Insertion Sort)
    for (int i = 1; i < this->cantidadVertices; i++)
    {
        Estacion *clave = listaOrdenada[i];
        int j = i - 1;

        while (j >= 0)
        {
            bool debeIntercambiar = false;

            if (tipoListado == 0)
            {
                // Mayor a menor por Capacidad de Anclajes
                if (listaOrdenada[j]->getCapacidad() < clave->getCapacidad())
                {
                    debeIntercambiar = true;
                }
            }
            else if (tipoListado == 1)
            {
                // Mayor a menor por Bicicletas Disponibles
                if (listaOrdenada[j]->getBicisDisp() < clave->getBicisDisp())
                {
                    debeIntercambiar = true;
                }
            }

            if (debeIntercambiar)
            {
                listaOrdenada[j + 1] = listaOrdenada[j];
                j--;
            }
            else
            {
                break;
            }
        }
        listaOrdenada[j + 1] = clave;
    }

    // 3. Imprimir por pantalla
    if (tipoListado == 0)
    {
        cout << "\n=== ESTACIONES ORDENADAS POR CAPACIDAD DE ANCLAJES (MAYOR A MENOR) ===\n";
    }
    else
    {
        cout << "\n=== ESTACIONES ORDENADAS POR BICICLETAS DISPONIBLES (MAYOR A MENOR) ===\n";
    }

    for (int i = 0; i < this->cantidadVertices; i++)
    {
        cout << i + 1 << ". ";
        listaOrdenada[i]->mostrarInfo(); // Muestra los datos de la estación
    }

    // 4. Liberar memoria dinámica auxiliar
    delete[] listaOrdenada;
};

void Grafo::listarEstacionesBajaOcupacion()
{
    if (this->vertices == nullptr || this->cantidadVertices <= 0)
    {
        cout << "No hay estaciones registradas en el sistema.\n";
        return;
    }

    bool hayEstacionesCriticas = false;

    cout << "\n=== ESTACIONES CON MENOS DEL 20% DE OCUPACION ===\n";

    for (int i = 0; i < this->cantidadVertices; i++)
    {
        Estacion *est = this->vertices[i].estacion;

        if (est != nullptr && est->getCapacidad() > 0)
        {
            // Evaluación con enteros: (bicis * 100) < (capacidad * 20)
            if (est->getBicisDisp() * 100 < est->getCapacidad() * 20)
            {
                int porcentaje = (est->getBicisDisp() * 100) / est->getCapacidad();

                cout << "- Estacion: " << est->getCodigo()
                     << " | Disponibles: " << est->getBicisDisp()
                     << " / Capacidad: " << est->getCapacidad()
                     << " (" << porcentaje << "% de ocupacion)\n";

                hayEstacionesCriticas = true;
            }
        }
    }

    if (!hayEstacionesCriticas)
    {
        cout << "Todas las estaciones operan con una ocupacion igual o superior al 20%.\n";
    }
}

void Grafo::BFS(const string &origen, const string &destino)
{
    int idOrigen = buscarId(origen);
    int idDestino = buscarId(destino);

    if (idOrigen == -1 || idDestino == -1)
    {
        cout << "Una de las estaciones no existe." << endl;
        return;
    }

    Cola cola;
    //lista de visitados-los inicializamos en no visitados(false)
    bool *visitados = new bool[cantidadVertices];
    int *predecesor = new int[cantidadVertices];

    for (int i = 0; i < cantidadVertices; i++)
    {
        visitados[i] = false;
        predecesor[i] = -1;
    }
    cola.agregar(idOrigen);
    visitados[idOrigen] = true;
    while(!cola.estaVacia()){
        int actual = cola.verPrimero();
        cola.quitar();
        //comprobamos si llegamos al destino
        if (actual == idDestino){
            cout << "Destino encontrado" <<endl;
            break;
        }
        Nodo *vecino = vertices[actual].adyacentes;
        while (vecino != nullptr){
            int idVecino = vecino->idVertice;
            if (!visitados[idVecino]){
                visitados[idVecino] = true;
                predecesor[idVecino] = actual;
                cola.agregar(idVecino);
            }
        vecino = vecino ->siguiente;
        }        
    }
    //Almacenamos el camino
    if (visitados[idDestino])
    {
        int *camino = new int[cantidadVertices];
        int longitud = 0;

        int actual = idDestino;

        while (actual != -1)
        {
            camino[longitud] = actual;
            longitud++;

            actual = predecesor[actual];
        }
        cout << "Recorrido: ";

        for (int i = longitud - 1; i >= 0; i--)
        {
            cout << vertices[camino[i]].estacion->getCodigo();

            if (i > 0)
            {
                cout << " -> ";
            }
        }

        cout << "\nCantidad de tramos: " << longitud - 1 << endl;
        
        delete[] camino;
    }
    else
    {
        cout << "No existe un recorrido entre las estaciones." << endl;
    }

    delete[] predecesor;
    delete[] visitados;
}
