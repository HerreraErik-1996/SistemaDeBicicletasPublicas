#include <string>
#include "Ciclovia.h"
using namespace std;

Ciclovia::Ciclovia(const std::string &estacionA, const std::string &estacionB){
        this->estacionA = estacionA;
        this->estacionB = estacionB;
    }

    string Ciclovia::getEstacionA() const{
        return estacionA;
    }
    string Ciclovia::getEstacionB() const{
        return estacionB;
    }

    void Ciclovia::setEstacionA(const string &nuevaEstacionA){
        estacionA = nuevaEstacionA;
    }
    void Ciclovia::setEstacionB(const string &nuevaEstacionB){
        estacionB = nuevaEstacionB;
    }

Ciclovia::~Ciclovia(){};
