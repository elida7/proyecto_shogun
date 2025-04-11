#ifndef LISTA_CONTRIBUIDORES_H
#define LISTA_CONTRIBUIDORES_H

#include "nodo_contribuidor.h"

// Clase para manejar la lista enlazada de contribuidores
class ListaContribuidores {
private:
    NodoContribuidor* cabeza; // Puntero al primer elemento
    
public:
    // Constructor inicializa lista vacía
    ListaContribuidores() : cabeza(nullptr) {}
    
    // Insertar contribuidor manteniendo orden por grado y edad
    void insertarOrdenado(std::string nombre, int edad, int id, 
                         std::string descripcion, int grado) {
        NodoContribuidor* nuevo = new NodoContribuidor(nombre, edad, id, descripcion, grado);
        
        // Caso 1: Lista vacía o nuevo es el más importante
        if (!cabeza || grado > cabeza->grado || 
            (grado == cabeza->grado && edad > cabeza->edad)) {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
            return;
        }
        
        // Caso 2: Buscar posición correcta
        NodoContribuidor* actual = cabeza;
        while (actual->siguiente && 
              (actual->siguiente->grado > grado || 
              (actual->siguiente->grado == grado && actual->siguiente->edad > edad))) {
            actual = actual->siguiente;
        }
        
        // Insertar después de actual
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
    
    // Mostrar todos los contribuidores
    void mostrar() {
        NodoContribuidor* actual = cabeza;
        if (!actual) {
            std::cout << "No hay contribuidores registrados\n";
            return;
        }
        
        std::cout << "Lista de contribuidores:\n";
        while (actual) {
            std::cout << "  - " << actual->nombre << " (" << actual->edad 
                      << " años, Grado " << actual->grado << "): " 
                      << actual->descripcion << "\n";
            actual = actual->siguiente;
        }
    }
    
    // Destructor - liberar memoria
    ~ListaContribuidores() {
        NodoContribuidor* actual = cabeza;
        while (actual) {
            NodoContribuidor* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

#endif