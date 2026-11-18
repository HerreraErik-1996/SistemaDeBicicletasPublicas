# Sistema de Gestión de Bicicletas

Sistema integral para la gestión de estaciones, red de ciclovías, flota de bicicletas, historial de viajes y taller de reparaciones, desarrollado en **C++17** bajo el paradigma de Programación Orientada a Objetos y gestión manual de memoria dinámica.

## Instrucciones de Compilación y Ejecución

### Requisitos previos

- Compilador `g++` con soporte para el estándar **C++17**.
- **Valgrind** (opcional, recomendado para verificar la ausencia de pérdidas de memoria en entornos Linux/WSL).

El proyecto está configurado para compilarse sin advertencias (_warnings_) utilizando las banderas `-Wall` y `-Wextra`.

### En Linux / macOS / WSL

1. **Compilar:** Desde la raíz del proyecto, ejecutar:

   ```bash
   g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o programa
   ```

2. **Ejecutar:**

   ```bash
   ./programa
   ```

3. **Verificación de Memoria (Valgrind):** Para comprobar que la gestión de memoria dinámica (punteros, nodos y arreglos dinámicos) no presente fugas (_memory leaks_):

   ```bash
   valgrind --leak-check=full --show-leak-kinds=all ./programa
   ```

### En Windows (MinGW / MSYS2)

1. **Compilar:** Desde la raíz del proyecto, ejecutar:

   ```bash
   g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o programa.exe
   ```

2. **Ejecutar:**

   ```bash
   programa.exe
   ```

   _(En PowerShell utilizar `.\programa.exe`)_

> **Nota sobre Valgrind:** Valgrind no está disponible de forma nativa en Windows. Para verificar fugas de memoria desde Windows, se recomienda compilar y ejecutar las instrucciones de Linux dentro de **WSL (Windows Subsystem for Linux)**.

## Descripción de Módulos

El sistema está diseñado bajo el paradigma de Orientación a Objetos en C++17, sin uso de contenedores de la STL y gestionando la memoria dinámica manualmente. La arquitectura se apoya en un esquema de **propiedad compartida mediante punteros**: las entidades principales (`Estacion` y `Bicicleta`) se instancian una única vez en memoria dinámica y sus direcciones son referenciadas desde las distintas estructuras de datos para evitar duplicación de información y mantener la consistencia de estado en tiempo real.

---

### Módulo de Control e Interfaz (`Sistema`)

- **Archivos:** `Sistema.h`, `Sistema.cpp`, `main.cpp`
- **Descripción:** Actúa como controlador central y fachada del programa. Se encarga de la lectura inicial de los archivos de datos (`estaciones.txt`, `bicicletas.txt`, `ciclovias.txt`, `viajes.txt`), la vinculación de las entidades entre las distintas estructuras y la gestión de la interfaz de consola organizada en submenús modulares.

---

### Módulo A: Gestión de Estaciones y Red de Ciclovías

- **Archivos:** `TablaHashEstacion.h`, `grafo.h`, `Grafo.cpp`, `estacion.h`, `Estacion.cpp`, `ciclovia.h`, `Ciclovia.cpp`, `cola.h`
- **Descripción:** Administra las estaciones de la ciudad y sus conexiones viales bidireccionales mediante dos estructuras sincronizadas que apuntan a las mismas instancias de `Estacion`:
  - **Índice de Estaciones (`TablaHashEstaciones`):** Tabla de hash abierta con resolución de colisiones por encadenamiento separado (_Separate Chaining_), tamaño primo por defecto (`17`) y función hash `djb2`. Permite realizar búsquedas, altas y bajas de estaciones por código en tiempo promedio $O(1)$.
  - **Red Vial (`Grafo`):** Grafo no dirigido implementado mediante un arreglo dinámico redimensionable de vértices y listas enlazadas de adyacencia. Incluye:
    - **Ordenamiento manual:** Implementación de _Insertion Sort_ sobre un arreglo auxiliar de punteros para listar estaciones por capacidad de anclajes o bicicletas disponibles sin alterar el grafo.
    - **Monitoreo de ocupación:** Filtrado de estaciones críticas con menos del 20% de disponibilidad.
    - **Camino más corto (`BFS`):** Búsqueda en anchura apoyada en una estructura `Cola` propia y arreglos de visitados/predecesores para determinar y reconstruir el recorrido con menor cantidad de tramos entre dos estaciones.

---

### Módulo B: Viajes, Colas de Espera y Taller de Reparaciones

- **Archivos:** `viaje.h`, `ListaViaje.cpp`, `estacion.h`, `Estacion.cpp`, `taller.h`, `taller.cpp`
- **Descripción:** Gestiona el flujo operativo de préstamos, historial de recorridos y mantenimiento de la flota:
  - **Historial de Viajes (`ListaViajes`):** Lista enlazada simple con inserción en cabeza ($O(1)$) que almacena los registros históricos y permite consultar la cantidad de viajes originados en una estación dentro de un rango de fechas (`AAAAMMDD`), así como listar los viajes y calcular los minutos totales pedaleados por un usuario según su DNI.
  - **Cola de Espera por Estación (`Estacion`):** Cada estación administra internamente una lista enlazada de bicicletas ancladas y una cola FIFO (`frenteCola` y `finCola`). Al solicitar una bicicleta sin stock disponible, el usuario se encola en $O(1)$; al devolverse una unidad, se asigna inmediatamente al primer usuario en espera.
  - **Taller de Reparaciones (`MinHeapTaller`):** Cola de prioridad implementada como un _Min-Heap_ binario sobre un arreglo dinámico redimensionable. Gestiona prioridades del 1 (urgente) al 3 (baja), forzando automáticamente prioridad `1` para bicicletas de tipo `"Electrica"`. Al ingresar una bicicleta se actualiza su estado a `"En Taller"` mediante `flotar()`, y al extraer la más urgente con `hundir()` se restaura su estado a `"OK"`.

---

### Módulo C: Registro de Bicicletas (ABB)

- **Archivos:** `ArbolBicicletas.h`, `ArbolBicicletas.cpp`, `bicicleta.h`, `Bicicleta.cpp`
- **Descripción:** Almacena la totalidad de la flota en un Árbol Binario de Búsqueda (`ArbolBicicletas`) ordenado por `bici_id`.
  - **Operaciones principales:** Inserción recursiva, búsqueda por ID, listado ordenado mediante recorrido _inorder_, conteo recursivo de unidades averiadas, filtro de bicicletas que superan un umbral de $X$ kilómetros para revisión técnica y cálculo de la altura del árbol.
  - **Eliminación y Gestión de Memoria:** Implementa la baja de nodos contemplando los tres casos del ABB (nodo hoja, un hijo y dos hijos mediante el sucesor _inorder_) y libera toda la memoria dinámica en el destructor mediante un recorrido _postorder_ (`destruirRec`).
  - **Sincronización:** Mediante `asignarAEstaciones()`, recorre el árbol en _inorder_ y vincula los punteros de las bicicletas operativas con sus respectivas estaciones en la Tabla Hash.

## Consignas de Análisis

### Análisis B: Cálculo de Complejidad Temporal (Big O)

#### Punto B.1: Conteo de viajes en rango de fechas

```cpp
int ListaViajes::contarViajesPorEstacionYRango(const string &codigoEstacion, int fechaInicio, int fechaFin)
{
    int contador = 0;                                                // 2 (declaración + asignación)
    NodoViaje *actual = cabeza;                                      // 2 (declaración + asignación)

    while (actual != nullptr)                                        // N + 1 (N comparaciones verdaderas + 1 falsa al final)
    {
        if (actual->datos.estacionOrigen == codigoEstacion &&        // N * 3 (acceso puntero, acceso struct, comparación)
            actual->datos.fecha >= fechaInicio &&                    // N * 3 (acceso puntero, acceso struct, comparación)
            actual->datos.fecha <= fechaFin)                         // N * 3 (acceso puntero, acceso struct, comparación)
        {
            contador++;                                              // N * 2 (lectura + escritura)
        }
        actual = actual->siguiente;                                  // N * 3 (acceso puntero, acceso miembro, asignación)
    }

    return contador;                                                 // 1 (retorno)
}
```

- **Función de tiempo:** $T(N) = 2 + 2 + (N + 1) + 3N + 3N + 3N + 2N + 3N + 1$
- **Simplificación:** $T(N) = 15N + 6$
- **Notación Big O:** $O(N)$

#### Punto B.2: Búsqueda de viajes por DNI y suma total de minutos

```cpp
void ListaViajes::buscarViajesPorDNI(const string &dniBuscado)
{
    int totalMinutos = 0;                                                    // 2 (declaración + asignación)
    bool encontrado = false;                                                 // 2 (declaración + asignación)
    NodoViaje *actual = cabeza;                                              // 2 (declaración + asignación)

    cout << "\nViajes del usuario DNI " << dniBuscado << ":\n";              // 3 (3 operaciones de salida <<)
    cout << "----------------------------------------\n";                    // 1 (1 operación de salida <<)

    while (actual != nullptr)                                                // N + 1 (N comparaciones verdaderas + 1 falsa)
    {
        if (actual->datos.dniUsuario == dniBuscado)                          // N * 3 (acceso puntero, acceso struct, comparación)
        {
            cout << "ID: " << actual->datos.viajeId                          // N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " | " << actual->datos.estacionOrigen                    // N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " -> " << actual->datos.estacionDestino                  // N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " | Fecha: " << actual->datos.fecha                      // N * 4 (2 salidas <<, acceso puntero, acceso struct)
                 << " | Duracion: " << actual->datos.duracionMin << " min\n"; // N * 5 (3 salidas <<, acceso puntero, acceso struct)

            totalMinutos += actual->datos.duracionMin;                       // N * 4 (acceso puntero, acceso struct, suma, asignación)
            encontrado = true;                                               // N * 1 (asignación)
        }
        actual = actual->siguiente;                                          // N * 3 (acceso puntero, acceso miembro, asignación)
    }

    if (!encontrado)                                                         // 1 (comparación lógica)
    {
        cout << "No se registraron viajes para el DNI " << dniBuscado << ".\n"; // 0 (no se ejecuta en el peor caso)
    }
    else
    {
        cout << "----------------------------------------\n";                // 1 (1 operación de salida <<)
        cout << "Total tiempo pedaleado: " << totalMinutos << " minutos.\n"; // 3 (3 operaciones de salida <<)
    }
}
```

- **Función de tiempo:** $T(N) = 2 + 2 + 2 + 3 + 1 + (N + 1) + 3N + 4N + 4N + 4N + 4N + 5N + 4N + N + 3N + 1 + 1 + 3$
- **Simplificación:** $T(N) = 33N + 16$
- **Notación Big O:** $O(N)$

#### Punto B.3: Filtro por Kilómetros

**Método recursivo auxiliar:**

```cpp
void ArbolBicicletas::listarMasDeXKmRec(NodoBici *nodo, int limiteKm, bool &seEncontraron) const
{
    if (nodo == nullptr)                                                    // 2N + 1 (se evalúa en todas las llamadas: N nodos + N+1 nulos)
        return;                                                             // N + 1 (se ejecuta solo cuando es nullptr)

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
```

- **Costo recursivo:** $T_{rec}(N) = (2N + 1) + (N + 1) + 3N + 4N + 5N + 5N + 5N + 6N + 6N + N + 3N = 41N + 2$

**Método principal:**

```cpp
void ArbolBicicletas::listarBicicletasMasDeXKm(int limiteKm) const
{
    bool seEncontraron = false;                                             // 2 (declaración + asignación)
    cout << "\n--- BICICLETAS CON MÁS DE " << limiteKm << " KM ---\n";      // 3 (3 operaciones de salida <<)
    listarMasDeXKmRec(raiz, limiteKm, seEncontraron);                       // 1 + (41N + 2) (llamada + costo recursivo)

    if (!seEncontraron)                                                     // 1 (comparación lógica)
    {
        cout << "No se encontraron bicicletas que superen los " << limiteKm << " km.\n"; // 0 (no entra en el peor caso)
    }
}
```

- **Función de tiempo:** $T(N) = 2 + 3 + 1 + (41N + 2) + 1$
- **Simplificación:** $T(N) = 41N + 9$
- **Notación Big O:** $O(N)$

#### Punto B.4: Gestión de Estaciones y Colas de Espera

**1. `estaVaciaCola`**

```cpp
bool Estacion::estaVaciaCola() const
{
    return frenteCola == nullptr;                           // 2 (comparación + retorno)
}
```

- **Función de tiempo:** $T(N) = 2$
- **Notación Big O:** $O(1)$

**2. `encolarUsuario`**

```cpp
void Estacion::encolarUsuario(const string &dniUsuario)
{
    NodoCola *nuevo = new NodoCola;                         // 3 (declaración, reserva de memoria new, asignación)
    nuevo->dniUsuario = dniUsuario;                         // 3 (acceso puntero, acceso miembro, asignación)
    nuevo->siguiente = nullptr;                             // 3 (acceso puntero, acceso miembro, asignación)

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
```

- **Función de tiempo:** $T(N) = 3 + 3 + 3 + 3 + 3 + 1 = 16$
- **Notación Big O:** $O(1)$

**3. `desencolarUsuario`**

```cpp
string Estacion::desencolarUsuario()
{
    if (estaVaciaCola())                                    // 2 (llamada + comparación)
    {
        return "";                                          // 0 (peor caso: la cola no está vacía)
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
```

- **Función de tiempo:** $T(N) = 3 + 2 + 4 + 3 + 1 + 1 + 1 + 1 = 16$
- **Notación Big O:** $O(1)$

**4. `agregarBici`**

```cpp
bool Estacion::agregarBici(Bicicleta *bici)
{
    if (bicisDisp >= capAnclaje || bici == nullptr)         // 3 (2 comparaciones + 1 operador lógico ||)
    {
        return false;                                       // 0 (peor caso: hay lugar y se inserta el nodo)
    }
    NodoBicicleta *nueva = new NodoBicicleta;               // 3 (declaración, new, asignación)
    nueva->bicicleta = bici;                                // 3 (acceso puntero, acceso miembro, asignación)
    nueva->siguiente = primeraBicicleta;                    // 3 (acceso puntero, acceso miembro, asignación)
    primeraBicicleta = nueva;                               // 1 (asignación)
    bicisDisp++;                                            // 2 (lectura + escritura)
    return true;                                            // 1 (retorno)
}
```

- **Función de tiempo:** $T(N) = 3 + 3 + 3 + 3 + 1 + 2 + 1 = 16$
- **Notación Big O:** $O(1)$

**5. `solicitarBicicleta`**

```cpp
Bicicleta* Estacion::solicitarBicicleta(const string &dniUsuario)
{
    // Caso 1: Hay bicicletas disponibles
    if (bicisDisp > 0 && primeraBicicleta != nullptr)       // 3 (2 comparaciones + 1 operador &&)
    {
        NodoBicicleta *aux = primeraBicicleta;              // 0 (no se ejecuta en Caso 2; en Caso 1 todo este bloque suma 16)
        Bicicleta *biciRetirada = aux->bicicleta;           // 0
        primeraBicicleta = primeraBicicleta->siguiente;     // 0
        delete aux;                                         // 0
        bicisDisp--;                                        // 0

        cout << "Bicicleta asignada con exito al usuario DNI: " << dniUsuario << endl; // 0
        return biciRetirada;                                // 0
    }

    // Caso 2: No hay bicicletas disponibles, se encola al usuario
    encolarUsuario(dniUsuario);                             // 1 + 15 (llamada + costo de encolarUsuario)
    cout << "Sin bicis disponibles en " << codigo << ". Usuario DNI " << dniUsuario // 5 (5 salidas <<)
         << " ingresado a la cola de espera.\n";            // 1 (1 salida <<)
    return nullptr;                                         // 1 (retorno)
}
```

- **Función de tiempo:** $T(N) = 3 + 17 + 6 + 1 + 1 = 28$ _(en Caso 1 sería_ $3 + 16 = 19$_)_
- **Notación Big O:** $O(1)$

**6. `devolverBicicleta`**

```cpp
void Estacion::devolverBicicleta(Bicicleta *bici)
{
    if (bici == nullptr)                                    // 1 (comparación)
        return;                                             // 0 (peor caso: bici válida)

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
```

- **Función de tiempo:** $T(N) = 1 + 4 + 19 + 29 + 3 + 1 = 57$
- **Notación Big O:** $O(1)$

#### Punto B.5: Gestión de Taller (MinHeap)

**1. `redimensionar`**

```cpp
void MinHeapTaller::redimensionar(int nuevaCapacidad)
{
    ElementoTaller *nuevoHeap = new ElementoTaller[nuevaCapacidad]; // 3 (declaración + reserva arreglo + asignación)
    for (int i = 0; i < cantidad; i++)                              // 2 + (N + 1) + 2N (init + N+1 comp + N inc)
    {
        nuevoHeap[i] = heap[i];                                     // N * 3 (acceso arreglo + acceso arreglo + asignación)
    }
    delete[] heap;                                                  // 1 (liberación de memoria)
    heap = nuevoHeap;                                               // 1 (asignación)
    capacidad = nuevaCapacidad;                                     // 1 (asignación)
}
```

- **Función de tiempo:** $T(N) = 3 + 3 + 3N + 3N + 1 + 1 + 1 = 6N + 9$
- **Notación Big O:** $O(N)$

**2. `flotar`**

```cpp
void MinHeapTaller::flotar(int i)
{
    while (i > 0)                                                   // log2(N) + 1 (comparaciones)
    {
        int padre = (i - 1) / 2;                                    // log2(N) * 4 (declaración + resta + div + asignación)
        if (heap[i].prioridad < heap[padre].prioridad)              // log2(N) * 5 (2 accesos arreglo + 2 miembros + comp)
        {
            swap(heap[i], heap[padre]);                             // log2(N) * 5 (2 accesos arreglo + 3 op del swap)
            i = padre;                                              // log2(N) * 1 (asignación)
        }
        else
        {
            break;                                                  // 0 (no entra en el peor caso)
        }
    }
}
```

- **Función de tiempo:** $T(N) = (\log_2 N + 1) + 4\log_2 N + 5\log_2 N + 5\log_2 N + \log_2 N = 16\log_2 N + 1$
- **Notación Big O:** $O(\log N)$

**3. `hundir`**

```cpp
void MinHeapTaller::hundir(int i)
{
    while (2 * i + 1 < cantidad)                                    // 3 * (log2(N) + 1) (mult + suma + comp)
    {
        int hijoIzq = 2 * i + 1;                                    // log2(N) * 4 (declaración + mult + suma + asignación)
        int hijoDer = 2 * i + 2;                                    // log2(N) * 4 (declaración + mult + suma + asignación)
        int menor = hijoIzq;                                        // log2(N) * 2 (declaración + asignación)

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
```

- **Función de tiempo:** $T(N) = (3\log_2 N + 3) + 4\log_2 N + 4\log_2 N + 2\log_2 N + 7\log_2 N + \log_2 N + 5\log_2 N + 5\log_2 N + \log_2 N = 32\log_2 N + 3$
- **Notación Big O:** $O(\log N)$

**4. `insertarBicicleta`**

```cpp
void MinHeapTaller::insertarBicicleta(Bicicleta *bici, int prioridadIngresada)
{
    if (bici == nullptr)                                            // 1 (comparación)
        return;                                                     // 0

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
```

- **Función de tiempo:** $T(N) = 6N + 16\log_2 N + 48$
- **Notación Big O:** $O(N)$

**5. `extraerMasUrgente`**

```cpp
Bicicleta *MinHeapTaller::extraerMasUrgente()
{
    if (estaVacio())                                                // 3 (1 llamada + 2 costo interno asumido)
    {
        cout << "El taller no tiene bicicletas pendientes de reparacion.\n"; // 0
        return nullptr;                                             // 0
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
```

- **Función de tiempo:** $T(N) = 3 + 4 + 4 + 2 + 1 + (32\log_2 N + 4) + 2 + 8 + 1 + 1 = 32\log_2 N + 30$
- **Notación Big O:** $O(\log N)$

### Análisis C: Comportamiento y Balanceo del Árbol Binario de Búsqueda (ABB)

- **Altura resultante:** Su altura sería de **5**, porque al ingresarse los elementos de forma ascendente basándose en el `ID` y al no contar con un mecanismo de autobalanceo, cada nuevo nodo se insertará siempre como hijo derecho del anterior. Al final, en lugar de mantener una estructura ramificada de árbol, termina degenerando y comportándose como una **lista enlazada**.
- **Impacto en el rendimiento:** Esto afecta su rendimiento de la peor manera posible, provocando que la operación _"Buscar bicicleta"_ en el peor de los casos (por ejemplo, buscar la bicicleta con el `ID` más grande) tenga que recorrer la totalidad de los elementos del árbol ($O(N)$).
- **Comparativa con un árbol balanceado:** Si el árbol estuviese balanceado, por cada nodo visitado se descartaría la mitad de los nodos restantes por recorrer ($O(\log N)$). En el ejemplo de las 5 bicicletas, en un árbol balanceado solo habría que analizar como máximo **3 nodos** en lugar de los **5** del caso degenerado.
