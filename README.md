CONSIGNA DE ANALISIS B.
Punto B.1: Conteo de viajes en rango de fechas
int ListaViajes::contarViajesPorEstacionYRango(const string &codigoEstacion, int fechaInicio, int fechaFin)
{
int contador = 0; 2 (declaración + asignación)
NodoViaje \*actual = cabeza; 2 (declaración + asignación)

    while (actual != nullptr)                                        N + 1 (N comparaciones verdaderas + 1 falsa al final)
    {
        if (actual->datos.estacionOrigen == codigoEstacion &&        N * 3 (acceso puntero, acceso struct, comparación)
            actual->datos.fecha >= fechaInicio &&                    N * 3 (acceso puntero, acceso struct, comparación)
            actual->datos.fecha <= fechaFin)                         N * 3 (acceso puntero, acceso struct, comparación)
        {
            contador++;                                              N * 2 (lectura + escritura)
        }
        actual = actual->siguiente;                                  N * 3 (acceso puntero, acceso miembro, asignación)
    }

    return contador;                                                 1 (retorno)

}
T(N) = 2 + 2 + (N + 1) + 3N + 3N + 3N + 2N + 3N + 1
T(N) = 15N + 6
Notación Big O: O(N)

Punto B.2: Búsqueda de viajes por DNI y suma total de minutos
void ListaViajes::buscarViajesPorDNI(const string &dniBuscado)
{
int totalMinutos = 0; 2 (declaración + asignación)
bool encontrado = false; 2 (declaración + asignación)
NodoViaje \*actual = cabeza; 2 (declaración + asignación)

    cout << "\nViajes del usuario DNI " << dniBuscado << ":\n";          3 (3 operaciones de salida <<)
    cout << "----------------------------------------\n";                1 (1 operación de salida <<)

    while (actual != nullptr)                                            N + 1 (N comparaciones verdaderas + 1 falsa)
    {
        if (actual->datos.dniUsuario == dniBuscado)                      N * 3 (acceso puntero, acceso struct, comparación)
        {
            cout << "ID: " << actual->datos.viajeId                      N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " | " << actual->datos.estacionOrigen                N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " -> " << actual->datos.estacionDestino              N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " | Fecha: " << actual->datos.fecha                  N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " | Duracion: " << actual->datos.duracionMin << " min\n";  N * 5 (3 salidas <<, acceso puntero, acceso struct)

            totalMinutos += actual->datos.duracionMin;                   N * 4 (acceso puntero, acceso struct, suma, asignación)
            encontrado = true;                                           N * 1 (asignación)
        }
        actual = actual->siguiente;                                      N * 3 (acceso puntero, acceso miembro, asignación)
    }

    if (!encontrado)                                                     1 (comparación lógica)
    {
        cout << "No se registraron viajes para el DNI " << dniBuscado << ".\n";  0 (no se ejecuta en el peor caso)
    }
    else
    {
        cout << "----------------------------------------\n";            1 (1 operación de salida <<)
        cout << "Total tiempo pedaleado: " << totalMinutos << " minutos.\n";  3 (3 operaciones de salida <<)
    }

}
T(N) = 2 + 2 + 2 + 3 + 1 + (N + 1) + 3N + 4N + 4N + 4N + 4N + 5N + 4N + N + 3N + 1 + 1 + 3
T(N) = 33N + 16
Notación Big O: O(N)

// B.3 Filtro por Kilómetros
void ArbolBicicletas::listarMasDeXKmRec(NodoBici \*nodo, int limiteKm, bool &seEncontraron) const
{
if (nodo == nullptr) // 2N + 1 (se evalúa en todas las llamadas: N nodos + N+1 nulos)
return; // N + 1 (se ejecuta solo cuando es nullptr)

    listarMasDeXKmRec(nodo->izquierdo, limiteKm, seEncontraron);            // N * 3 (acceso puntero + acceso miembro + llamada recursiva)

    if (nodo->dato.kilometraje() > limiteKm)                                // N * 4 (acceso puntero, acceso dato, llamada getter, comparación)
    {
        cout << "- Bici ID: " << nodo->dato.getId()                         // N * 5 (2 salidas <<, acceso puntero, acceso dato, getter)
             << " | Estacion: " << nodo->dato.getCodigoEstacion()           // N * 5 (2 salidas <<, acceso puntero, acceso dato, getter)
             << " | Tipo: " << nodo->dato.getTipo()                         // N * 5 (2 salidas <<, acceso puntero, acceso dato, getter)
             << " | Kilometros: " << nodo->dato.kilometraje() << " km"      // N * 6 (3 salidas <<, acceso puntero, acceso dato, getter)
             << " | Estado: " << nodo->dato.getEstado() << "\n";            // N * 6 (3 salidas <<, acceso puntero, acceso dato, getter)
        seEncontraron = true;                                               // N * 1 (asignación)
    }

    listarMasDeXKmRec(nodo->derecho, limiteKm, seEncontraron);              // N * 3 (acceso puntero + acceso miembro + llamada recursiva)

}
T*{rec}(N) = (2N + 1) + (N + 1) + 3N + 4N + 5N + 5N + 5N + 6N + 6N + N + 3N
T*{rec}(N) = 41N + 2

void ArbolBicicletas::listarBicicletasMasDeXKm(int limiteKm) const
{
bool seEncontraron = false; // 2 (declaración + asignación)
cout << "\n--- BICICLETAS CON MÁS DE " << limiteKm << " KM ---\n"; // 3 (3 operaciones de salida <<)
listarMasDeXKmRec(raiz, limiteKm, seEncontraron); // 1 + (41N + 2) (llamada + costo recursivo)

    if (!seEncontraron)                                                     // 1 (comparación lógica)
    {
        cout << "No se encontraron bicicletas que superen los " << limiteKm << " km.\n"; // 0 (no entra en el peor caso)
    }

}
T(N) = 2 + 3 + 1 + (41N + 2) + 1
T(N) = 41N + 9
Notación Big O: O(N)

//B.4
bool Estacion::estaVaciaCola() const
{
return frenteCola == nullptr; // 2 (comparación + retorno)
}
T(N) = 2
Notación Big O: O(1)

void Estacion::encolarUsuario(const string &dniUsuario)
{
NodoCola \*nuevo = new NodoCola; // 3 (declaración, reserva de memoria new, asignación)
nuevo->dniUsuario = dniUsuario; // 3 (acceso puntero, acceso miembro, asignación)
nuevo->siguiente = nullptr; // 3 (acceso puntero, acceso miembro, asignación)

    if (estaVaciaCola())                                    // 2 (llamada + comparación)
    {
        frenteCola = nuevo;                                 // 0 (peor caso: entra por el else)
    }
    else
    {
        finCola->siguiente = nuevo;                         // 3 (acceso puntero, acceso miembro, asignación)
    }
    finCola = nuevo;                                        // 1 (asignación)

}
T(N) = 3 + 3 + 3 + 3 + 3 + 1 = 16
Notación Big O: O(1)

string Estacion::desencolarUsuario()
{
if (estaVaciaCola()) // 2 (llamada + comparación)
{
return ""; // 0 (peor caso: la cola no está vacía)
}

    NodoCola *auxiliar = frenteCola;                        // 2 (declaración + asignación)
    string dni = auxiliar->dniUsuario;                      // 4 (declaración, acceso puntero, acceso miembro, asignación)

    frenteCola = frenteCola->siguiente;                     // 3 (acceso puntero, acceso miembro, asignación)
    if (frenteCola == nullptr)                              // 1 (comparación)
    {
        finCola = nullptr;                                  // 1 (asignación, peor caso: era el último nodo)
    }

    delete auxiliar;                                        // 1 (liberación de memoria)
    return dni;                                             // 1 (retorno)

}
T(N) = 3 + 2 + 4 + 3 + 1 + 1 + 1 + 1 = 16
Notación Big O: O(1)

bool Estacion::agregarBici(Bicicleta *bici)
{
if (bicisDisp >= capAnclaje || bici == nullptr) // 3 (2 comparaciones + 1 operador lógico ||)
{
return false; // 0 (peor caso: hay lugar y se inserta el nodo)
}
NodoBicicleta *nueva = new NodoBicicleta; // 3 (declaración, new, asignación)
nueva->bicicleta = bici; // 3 (acceso puntero, acceso miembro, asignación)
nueva->siguiente = primeraBicicleta; // 3 (acceso puntero, acceso miembro, asignación)
primeraBicicleta = nueva; // 1 (asignación)
bicisDisp++; // 2 (lectura + escritura)
return true; // 1 (retorno)
}
T(N) = 3 + 3 + 3 + 3 + 1 + 2 + 1 = 16
Notación Big O: O(1)

Bicicleta* Estacion::solicitarBicicleta(const string &dniUsuario)
{
// Caso 1: Hay bicicletas disponibles
if (bicisDisp > 0 && primeraBicicleta != nullptr) // 3 (2 comparaciones + 1 operador &&)
{
NodoBicicleta *aux = primeraBicicleta; // 0 (no se ejecuta en Caso 2; en Caso 1 todo este bloque suma 16)
Bicicleta \*biciRetirada = aux->bicicleta; // 0
primeraBicicleta = primeraBicicleta->siguiente; // 0
delete aux; // 0
bicisDisp--; // 0

        cout << "Bicicleta asignada con exito al usuario DNI: " << dniUsuario << endl; // 0
        return biciRetirada;                                // 0
    }

    // Caso 2: No hay bicicletas disponibles, se encola al usuario
    encolarUsuario(dniUsuario);                             // 1 + 15 (llamada + costo de encolarUsuario)
    cout << "Sin bicis disponibles en " << codigo << ". Usuario DNI " << dniUsuario // 5 (5 salidas <<)
         << " ingresado a la cola de espera.\n";            // 1 (1 salida <<)
    return nullptr;                                         // 1 (retorno)

}
T(N) = 3 + 17 + 6 + 1 + 1 = 28 (en Caso 1 sería 3 + 16 = 19).
Notación Big O: O(1)

void Estacion::devolverBicicleta(Bicicleta \*bici)
{
if (bici == nullptr) // 1 (comparación)
return; // 0 (peor caso: bici válida)

    // Si hay gente esperando en la cola, la bicicleta pasa directamente al usuario
    if (!estaVaciaCola())                                   // 3 (llamada + comparación + negación !)
    {
        string dniSiguiente = desencolarUsuario();          // 3 + 15 (declaración, asignación, llamada + costo desencolar)
        solicitarBicicleta(dniSiguiente);                   // 1 + 26 (llamada + costo peor caso solicitarBicicleta)
        cout << "Bicicleta devuelta asignada inmediatamente a DNI: " << dniSiguiente // 3 (3 salidas <<)
             << " (estaba en cola de espera).\n";           // 1 (1 salida <<)
    }
    else
    {
        // Si no hay nadie en cola, se ancla en la estación
        if (agregarBici(bici))                              // 0 (no entra en peor caso; este bloque else sumaría 1 + 16 + 3 = 20)
        {
            cout << "Bicicleta devuelta y anclada correctamente en estacion " << codigo << ".\n"; // 0
        }
        else
        {
            cout << "Error: La estacion " << codigo << " no tiene anclajes libres disponibles.\n"; // 0
        }
    }

}
T(N) = 1 + 4 + 19 + 29 + 3 + 1 = 57
Notación Big O: O(1)

//B.5
void MinHeapTaller::redimensionar(int nuevaCapacidad)
{
ElementoTaller _nuevoHeap = new ElementoTaller[nuevaCapacidad]; // 3 (declaración + reserva arreglo + asignación)
for (int i = 0; i < cantidad; i++) // 2 + (N + 1) + 2N (init + N+1 comp + N inc)
{
nuevoHeap[i] = heap[i]; // N _ 3 (acceso arreglo + acceso arreglo + asignación)
}
delete[] heap; // 1 (liberación de memoria)
heap = nuevoHeap; // 1 (asignación)
capacidad = nuevaCapacidad; // 1 (asignación)
}
$T(N) = 3 + 3 + 3N + 3N + 1 + 1 + 1 = 6N + 9
Notación Big O: O(N)

void MinHeapTaller::flotar(int i)
{
while (i > 0) // log2(N) + 1 (comparaciones)
{
int padre = (i - 1) / 2; // log2(N) _ 4 (declaración + resta + div + asignación)
if (heap[i].prioridad < heap[padre].prioridad) // log2(N) _ 5 (2 accesos arreglo + 2 miembros + comp)
{
swap(heap[i], heap[padre]); // log2(N) _ 5 (2 accesos arreglo + 3 op del swap)
i = padre; // log2(N) _ 1 (asignación)
}
else
{
break; // 0 (no entra en el peor caso)
}
}
}
$T(N) = (\log_2 N + 1) + 4\log_2 N + 5\log_2 N + 5\log_2 N + \log_2 N = 16\log_2 N + 1 $
Notación Big O: O(log N)

void MinHeapTaller::hundir(int i)
{
while (2 _ i + 1 < cantidad) // 3 _ (log2(N) + 1) (mult + suma + comp)
{
int hijoIzq = 2 _ i + 1; // log2(N) _ 4 (declaración + mult + suma + asignación)
int hijoDer = 2 _ i + 2; // log2(N) _ 4 (declaración + mult + suma + asignación)
int menor = hijoIzq; // log2(N) \* 2 (declaración + asignación)

        if (hijoDer < cantidad && heap[hijoDer].prioridad < heap[hijoIzq].prioridad) // log2(N) * 7 (2 comp + lógico + 2 accesos + 2 miembros)
        {
            menor = hijoDer;                                        // log2(N) * 1 (asignación - peor caso)
        }

        if (heap[menor].prioridad < heap[i].prioridad)              // log2(N) * 5 (2 accesos arreglo + 2 miembros + comp)
        {
            swap(heap[i], heap[menor]);                             // log2(N) * 5 (2 accesos arreglo + 3 op del swap)
            i = menor;                                              // log2(N) * 1 (asignación)
        }
        else
        {
            break;                                                  // 0 (no entra en el peor caso)
        }
    }

}
$T(N) = (3\log_2 N + 3) + 4\log_2 N + 4\log_2 N + 2\log_2 N + 7\log_2 N + \log_2 N + 5\log_2 N + 5\log_2 N + \log_2 N = 32\log_2 N + 3 $
Notación Big O: O(log N)

void MinHeapTaller::insertarBicicleta(Bicicleta \*bici, int prioridadIngresada)
{
if (bici == nullptr) // 1 (comparación)
return; // 0

    if (cantidad == capacidad)                                      // 1 (comparación)
    {
        redimensionar(capacidad * 2);                               // 2 + (6N + 9) = 6N + 11 (mult + llamada + costo interno)
    }

    ElementoTaller nuevo;                                           // 1 (declaración)
    nuevo.bici = bici;                                              // 2 (acceso miembro + asignación)

    // Regla de negocio: si es Eléctrica entra siempre con prioridad 1
    if (bici->getTipo() == "Electrica" || bici->getTipo() == "electrica") // 7 (2 accesos puntero + 2 llamadas getter + 2 comp + lógico)
    {
        nuevo.prioridad = 1;                                        // 0 (no entra en el peor caso de ramas)
    }
    else
    {
        if (prioridadIngresada < 1)                                 // 1 (comparación)
            nuevo.prioridad = 1;                                    // 0
        else if (prioridadIngresada > 3)                            // 1 (comparación)
            nuevo.prioridad = 3;                                    // 0
        else
            nuevo.prioridad = prioridadIngresada;                  // 2 (acceso miembro + asignación - rama más profunda)
    }

    // Le cambiamos el estado a la bicicleta mediante su setter
    bici->setEstado("En Taller");                                   // 2 (acceso puntero + llamada setter)

    heap[cantidad] = nuevo;                                         // 2 (acceso arreglo + asignación)
    flotar(cantidad);                                               // 1 + (16 * log2(N) + 1) = 16 * log2(N) + 2
    cantidad++;                                                     // 2 (lectura + escritura)

    cout << "Bicicleta ID " << bici->getId() << " (" << bici->getTipo() // 8 (4 salidas << + 2 accesos puntero + 2 getters)
         << ") ingresada al taller con Prioridad " << nuevo.prioridad << ".\n"; // 4 (3 salidas << + acceso miembro)

}
$$T(N) = 6N + 16\log_2 N + 48 $$
Notación Big O: O(N)

Bicicleta \*MinHeapTaller::extraerMasUrgente()
{
if (estaVacio()) // 3 (1 llamada + 2 costo interno asumido)
{
cout << "El taller no tiene bicicletas pendientes de reparacion.\n"; // 0
return nullptr; // 0
}

    Bicicleta *masUrgente = heap[0].bici;                           // 4 (declaración + acceso arreglo + miembro + asignación)

    heap[0] = heap[cantidad - 1];                                   // 4 (acceso arreglo + resta + acceso arreglo + asignación)
    cantidad--;                                                     // 2 (lectura + escritura)

    if (cantidad > 0)                                               // 1 (comparación)
    {
        hundir(0);                                                  // 1 + (32 * log2(N) + 3) = 32 * log2(N) + 4
    }

    // Al salir del taller, le cambiamos el estado a OK
    masUrgente->setEstado("OK");                                    // 2 (acceso puntero + llamada setter)

    cout << "Bicicleta ID " << masUrgente->getId() << " (" << masUrgente->getTipo() // 8 (4 salidas << + 2 accesos puntero + 2 getters)
         << ") reparada.\n";                                       // 1 (1 salida <<)

    return masUrgente;                                              // 1 (retorno)

}
$$T(N) = 3 + 4 + 4 + 2 + 1 + (32\log_2 N + 4) + 2 + 8 + 1 + 1 = 32\log_2 N + 30 $$
Notación Big O: O(logN)
