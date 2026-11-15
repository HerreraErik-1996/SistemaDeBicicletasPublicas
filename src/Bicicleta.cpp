#include <string>
#include "bicicleta.h"
using namespace std;


Bicicleta::Bicicleta(int biciId, const string &codigoEstacion, const string &tipo, const string &estado, int kilometros)
{
    this->id = biciId;
    this->codigoEstacion = codigoEstacion;
    this->tipo = tipo;
    this->estado = estado;
    this->kilometros = kilometros;
}
int Bicicleta::getId() const
{
    return id;
}
string Bicicleta::getCodigoEstacion() const
{
    return codigoEstacion;
}
string Bicicleta::getTipo() const
{
    return tipo;
}
string Bicicleta::getEstado() const
{
    return estado;
}
bool Bicicleta::estaDisponible() const
{
    return estado == "OK";
}
int Bicicleta::kilometraje() const
{
    return kilometros;
}
void Bicicleta::setEstado(const string &nuevoEstado)
{
    estado = nuevoEstado;
}
void Bicicleta::setCodigoEstacion(const string &nuevoCodigo)
{
    codigoEstacion = nuevoCodigo;
}
void Bicicleta::agregarKilometros(int km)
{
    kilometros += km;
}