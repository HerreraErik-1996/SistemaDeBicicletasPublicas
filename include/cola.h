#pragma once
#include <iostream>
using namespace std;

class Cola{
private:
    struct Nodo{
        int dato;
        Nodo *siguiente;
    };

    Nodo *primero;
    Nodo *ultimo;

public:
    Cola(){
        primero = nullptr;
        ultimo = nullptr;
    }

    void agregar(int valor){
        Nodo *nuevo = new Nodo;
        nuevo->dato = valor;
        nuevo->siguiente = nullptr;

        if (primero == nullptr){
            primero = nuevo;
            ultimo = nuevo;
        }else{
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
    }
    // elimina el primer elemento de la cola
    bool quitar(){
        if (primero == nullptr){
            return false;
        }
        //para guardar el primero de la cola antes de reasignar el nuevo primero
        Nodo *auxiliar = primero;
        primero = primero->siguiente;

        if (primero == nullptr){
            ultimo = nullptr;
        }
        delete auxiliar;
        return true;
    }

    int verPrimero()
    {
        if (primero == nullptr)
        {
            return -1;
        }

        return primero->dato;
    }

    bool estaVacia()
    {
        return primero == nullptr;
    }

    // Destructor
    ~Cola()
    {
        while (!estaVacia())
        {
            quitar();
        }
    }
};