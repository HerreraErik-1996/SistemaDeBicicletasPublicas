#pragma once
#include <string>


class Bicicleta
{
private:
    int id;
    std::string codigoEstacion;
    std::string tipo;   // "Mecanica" o "Electrica"
    std::string estado; // "OK" o "Averiada"
    int kilometros;

public:
    Bicicleta(int biciId, const std::string &codigoEstacion, const std::string &tipo, const std::string &estado, int kilometros);

    int getId() const;
    std::string getCodigoEstacion() const;
    std::string getTipo() const;
    std::string getEstado() const;
    int kilometraje() const;

    bool estaDisponible() const;

    void setEstado(const std::string& nuevoEstado);
    void setCodigoEstacion(const std::string& nuevoCodigo);
    void agregarKilometros(int km);
};