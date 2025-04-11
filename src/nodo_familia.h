#ifndef NODO_FAMILIA_H
#define NODO_FAMILIA_H

#include "lista_contribuidores.h"

// Estructura  
struct NodoFamilia {
    int id;               
    std::string nombre;    
    std::string apellido;   
    char genero;           
    int edad;             
    int id_padre;        
    bool esta_muerto;    
    bool fue_lider;     
    bool es_lider;     
    
    // Para árbol AVL
    int altura;        
    NodoFamilia* izquierdo; 
    NodoFamilia* derecho;  
    
    // Para relaciones familiares
    NodoFamilia* padre;    
    NodoFamilia* primer_hijo; // Primer hijo (lista enlazada)
    NodoFamilia* siguiente_hermano; // Siguiente hermano
    
    ListaContribuidores contribuidores; // Lista de contribuidores
    
    // Constructor
    NodoFamilia(int i, std::string n, std::string a, char g, int e, 
               int ip, bool m, bool fl, bool el)
        : id(i), nombre(n), apellido(a), genero(g), edad(e), id_padre(ip),
          esta_muerto(m), fue_lider(fl), es_lider(el), altura(1),
          izquierdo(nullptr), derecho(nullptr), padre(nullptr),
          primer_hijo(nullptr), siguiente_hermano(nullptr) {}
    
    // Actualizar altura del nodo
    void actualizarAltura() {
        int altIzq = izquierdo ? izquierdo->altura : 0;
        int altDer = derecho ? derecho->altura : 0;
        altura = 1 + std::max(altIzq, altDer);
    }
    
    // Factor de balance
    int factorBalance() {
        int altIzq = izquierdo ? izquierdo->altura : 0;
        int altDer = derecho ? derecho->altura : 0;
        return altIzq - altDer;
    }
};

#endif