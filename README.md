B.1
int ListaViajes::contarViajesPorEstacionYRango(const string &codigoEstacion, int fechaInicio, int fechaFin)
{
    int contador = 0;           
    NodoViaje *actual = cabeza;

    while (actual != nullptr)
    {
        if (actual->datos.estacionOrigen == codigoEstacion &&
            actual->datos.fecha >= fechaInicio &&
            actual->datos.fecha <= fechaFin)
        {
            contador++;
        }
        actual = actual->siguiente;
    }

    return contador;
}

B.2
void ListaViajes::buscarViajesPorDNI(const string &dniBuscado)
{
    int totalMinutos = 0;
    bool encontrado = false;
    NodoViaje *actual = cabeza;

    cout << "\nViajes del usuario DNI " << dniBuscado << ":\n";
    cout << "----------------------------------------\n";

    while (actual != nullptr)
    {
        if (actual->datos.dniUsuario == dniBuscado)
        {
            cout << "ID: " << actual->datos.viajeId
                 << " | " << actual->datos.estacionOrigen
                 << " -> " << actual->datos.estacionDestino
                 << " | Fecha: " << actual->datos.fecha
                 << " | Duracion: " << actual->datos.duracionMin << " min\n";

            totalMinutos += actual->datos.duracionMin;
            encontrado = true;
        }
        actual = actual->siguiente;
    }

    if (!encontrado)
    {
        cout << "No se registraron viajes para el DNI " << dniBuscado << ".\n";
    }
    else
    {
        cout << "----------------------------------------\n";
        cout << "Total tiempo pedaleado: " << totalMinutos << " minutos.\n";
    }
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

B.4
bool Estacion::solicitarBicicleta(const string &dniUsuario)
{
    // Caso 1: Hay bicicletas disponibles
    if (bicisDisp > 0 && primeraBicicleta != nullptr)
    {
        NodoBicicleta *aux = primeraBicicleta;
        primeraBicicleta = primeraBicicleta->siguiente;
        delete aux; // Se retira de la pila local de la estación
        bicisDisp--;

        cout << "Bicicleta asignada con exito al usuario DNI: " << dniUsuario << endl;
        return true;
    }

    // Caso 2: No hay bicicletas disponibles, se encola al usuario
    encolarUsuario(dniUsuario);
    cout << "Sin bicis disponibles en " << codigo << ". Usuario DNI " << dniUsuario
         << " ingresado a la cola de espera.\n";
    return false;
}
void Estacion::encolarUsuario(const string &dniUsuario)
{
    NodoCola *nuevo = new NodoCola;
    nuevo->dniUsuario = dniUsuario;
    nuevo->siguiente = nullptr;

    if (estaVaciaCola())
    {
        frenteCola = nuevo;
    }
    else
    {
        finCola->siguiente = nuevo;
    }
    finCola = nuevo;
}


