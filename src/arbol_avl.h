#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include "nodo_familia.h"
#include <algorithm>

// Clase para el árbol AVL que indexa miembros por ID
class ArbolAVL {
private:
    NodoFamilia* raiz;
    
    // Rotación simple a la derecha
    NodoFamilia* rotarDerecha(NodoFamilia* y) {
        NodoFamilia* x = y->izquierdo;
        NodoFamilia* T2 = x->derecho;
        
        // Realizar rotación
        x->derecho = y;
        y->izquierdo = T2;
        
        // Actualizar alturas
        y->actualizarAltura();
        x->actualizarAltura();
        
        return x;
    }
    
    // Rotación simple a la izquierda
    NodoFamilia* rotarIzquierda(NodoFamilia* x) {
        NodoFamilia* y = x->derecho;
        NodoFamilia* T2 = y->izquierdo;
        
        // Realizar rotación
        y->izquierdo = x;
        x->derecho = T2;
        
        // Actualizar alturas
        x->actualizarAltura();
        y->actualizarAltura();
        
        return y;
    }
    
    // Insertar nodo en el árbol AVL
    NodoFamilia* insertar(NodoFamilia* nodo, NodoFamilia* nuevo) {
        // 1. Inserción estándar en BST
        if (!nodo) return nuevo;
        
        if (nuevo->id < nodo->id) {
            nodo->izquierdo = insertar(nodo->izquierdo, nuevo);
        } else if (nuevo->id > nodo->id) {
            nodo->derecho = insertar(nodo->derecho, nuevo);
        } else {
            return nodo; // IDs duplicados no permitidos
        }
        
        // 2. Actualizar altura del nodo actual
        nodo->actualizarAltura();
        
        // 3. Obtener factor de balance
        int balance = nodo->factorBalance();
        
        // 4. Casos de desbalance
        
        // Izquierda Izquierda
        if (balance > 1 && nuevo->id < nodo->izquierdo->id) {
            return rotarDerecha(nodo);
        }
        
        // Derecha Derecha
        if (balance < -1 && nuevo->id > nodo->derecho->id) {
            return rotarIzquierda(nodo);
        }
        
        // Izquierda Derecha
        if (balance > 1 && nuevo->id > nodo->izquierdo->id) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }
        
        // Derecha Izquierda
        if (balance < -1 && nuevo->id < nodo->derecho->id) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }
        
        return nodo;
    }
    
    // Buscar nodo por ID
    NodoFamilia* buscar(NodoFamilia* nodo, int id) const {
        if (!nodo) return nullptr;
        
        if (id < nodo->id) {
            return buscar(nodo->izquierdo, id);
        } else if (id > nodo->id) {
            return buscar(nodo->derecho, id);
        } else {
            return nodo;
        }
    }
    
    // Recorrido en orden (para mostrar miembros)
    void enOrden(NodoFamilia* nodo) const {
        if (!nodo) return;
        
        enOrden(nodo->izquierdo);
        
        std::cout << "- " << nodo->nombre << " " << nodo->apellido 
                  << " (ID: " << nodo->id << ", Edad: " << nodo->edad 
                  << ", " << (nodo->esta_muerto ? "Muerto" : "Vivo") << ")" 
                  << std::endl;
        
        enOrden(nodo->derecho);
    }
    
    // Función auxiliar para destruir el árbol
    void destruirArbol(NodoFamilia* nodo) {
        if (!nodo) return;
        
        destruirArbol(nodo->izquierdo);
        destruirArbol(nodo->derecho);
        
        // No eliminamos el nodo aquí porque es manejado por el árbol genealógico
    }
    
public:
    // Constructor
    ArbolAVL() : raiz(nullptr) {}
    
    // Destructor
    ~ArbolAVL() {
        destruirArbol(raiz);
    }
    
    // Insertar nuevo miembro
    void insertar(NodoFamilia* nuevo) {
        raiz = insertar(raiz, nuevo);
    }
    
    // Buscar miembro por ID
    NodoFamilia* buscar(int id) const {
        return buscar(raiz, id);
    }
    
    // Mostrar todos los miembros en orden
    void mostrarEnOrden() const {
        std::cout << "\nMiembros del Clan:\n";
        enOrden(raiz);
    }
};

#endif