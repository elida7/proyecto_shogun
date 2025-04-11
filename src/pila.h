#ifndef PILA_H
#define PILA_H

// Plantilla para nodo de pila
template <typename T>
struct NodoPila {
    T dato;             // Dato almacenado
    NodoPila<T>* abajo; // Puntero al nodo inferior
    
    // Constructor
    NodoPila(T d) : dato(d), abajo(nullptr) {}
};

// Plantilla para pila (LIFO)
template <typename T>
class Pila {
private:
    NodoPila<T>* tope; // Puntero al nodo superior
    
public:
    // Constructor
    Pila() : tope(nullptr) {}
    
    // Verificar si la pila está vacía
    bool estaVacia() const {
        return tope == nullptr;
    }
    
    // Apilar elemento
    void apilar(T dato) {
        NodoPila<T>* nuevo = new NodoPila<T>(dato);
        nuevo->abajo = tope;
        tope = nuevo;
    }
    
    // Desapilar elemento
    T desapilar() {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacía");
        }
        
        NodoPila<T>* temp = tope;
        T dato = temp->dato;
        tope = tope->abajo;
        delete temp;
        return dato;
    }
    
    // Ver tope sin desapilar
    T verTope() const {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacía");
        }
        return tope->dato;
    }
    
    // Destructor
    ~Pila() {
        while (!estaVacia()) {
            desapilar();
        }
    }
};

#endif
