#pragma once
#include <string>
#include <iostream>

class Ciclovia{
private:
    std::string estacionA = nullptr;
    std::string estacionB = nullptr;
    
public:
    Ciclovia(const std::string &estacionA, const std::string &estacionB);

    std::string getEstacionA() const;
    std::string getEstacionB() const;

    void setEstacionA(const std::string &nuevaEstacionA);
    void setEstacionB(const std::string &nuevaEstacionB);

    ~Ciclovia();
};