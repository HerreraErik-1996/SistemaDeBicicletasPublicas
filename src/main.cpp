#include <iostream>
#include "Sistema.h"
#include "CargadorDatos.h"

int main()
{

    Sistema sistema;
    CargadorDatos cargador;

    if (!cargador.cargarEstaciones(sistema))
    {
        return 1;
    }

    if (!cargador.cargarCiclovias(sistema))
    {
        return 1;
    }

    sistema.ejecutarMenu();

    return 0;
}