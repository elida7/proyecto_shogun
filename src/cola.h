#ifndef COLA_H
#define COLA_H

// Plantilla para nodo de cola
template <typename T>
struct NodoCola {
    T dato;             // Dato almacenado
    NodoCola<T>* siguiente; // Puntero al siguiente nodo
    
    // Constructor
    NodoCola(T d) : dato(d), siguiente(nullptr) {}
};

// Plantilla para cola (FIFO)
template <typename T>
class Cola {
private:
    NodoCola<T>* frente; // Puntero al frente de la cola
    NodoCola<T>* final;  // Puntero al final de la cola
    
public:
    // Constructor
    Cola() : frente(nullptr), final(nullptr) {}
    
    // Verificar si la cola está vacía
    bool estaVacia() const {
        return frente == nullptr;
    }
    
    // Encolar elemento
    void encolar(T dato) {
        NodoCola<T>* nuevo = new NodoCola<T>(dato);
        
        if (estaVacia()) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
    }
    
    // Desencolar elemento
    T desencolar() {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacía");
        }
        
        NodoCola<T>* temp = frente;
        T dato = temp->dato;
        frente = frente->siguiente;
        
        if (!frente) {
            final = nullptr;
        }
        
        delete temp;
        return dato;
    }
    
    // Ver frente sin desencolar
    T verFrente() const {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacía");
        }
        return frente->dato;
    }
    
    // Destructor
    ~Cola() {
        while (!estaVacia()) {
            desencolar();
        }
    }
};

#endif