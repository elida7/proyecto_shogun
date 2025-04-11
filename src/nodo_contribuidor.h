#ifndef NODO_CONTRIBUIDOR_H
#define NODO_CONTRIBUIDOR_H

#include <string>

// Estructura de un contribuidor del clan
struct NodoContribuidor {
    std::string nombre;      // Nombre 
    int edad;               // Edad 
    int id;                // Identificador 
    std::string descripcion;// Descripción de su contribución
    int grado;             // Grado de contribución (1-10)
    NodoContribuidor* siguiente; // Puntero al siguiente contribuidor
    
    // Constructor
    NodoContribuidor(std::string n, int e, int i, std::string d, int g)
        : nombre(n), edad(e), id(i), descripcion(d), grado(g), siguiente(nullptr) {}
};

#endif