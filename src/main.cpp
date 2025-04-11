   #include <iostream>
   #include <fstream>
   #include <sstream>
   #include <string>
   #include <vector>
   #include <stdexcept>
   #include <filesystem>
   #include <algorithm>
   #include "nodo_familia.h"
   #include "arbol_avl.h"
   #include "pila.h"
   #include "cola.h"
   
   namespace fs = std::filesystem;
   
   class SistemaClan {
   private:
       ArbolAVL arbolID;
       NodoFamilia* raizGenealogia;
       NodoFamilia* liderActual;
       
       std::string construirRuta(const std::string& rutaRelativa) {
           fs::path basePath = fs::current_path();
           while (basePath.filename() != "proyecto_shogun" && basePath.has_parent_path()) {
               basePath = basePath.parent_path();
           }
           return (basePath / rutaRelativa).string();
       }
   
       void cargarFamilia(const std::string& archivo) {
        std::string rutaAbsoluta = construirRuta(archivo);
        
        // Verificación adicional del archivo
        if (!fs::exists(rutaAbsoluta)) {
            throw std::runtime_error("Archivo no encontrado: " + rutaAbsoluta);
        }
    
        std::ifstream file(rutaAbsoluta);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir: " + rutaAbsoluta);
        }
        
        std::string linea;
        std::vector<NodoFamilia*> nodos;
        std::getline(file, linea);    // Leer cabecera
    
        
        while (std::getline(file, linea)) {
            try{
                std::stringstream ss(linea);
                 std::string token;
                std::vector<std::string> datos;

                while (std::getline(ss, token, ',')) {
                    datos.push_back(token);

            }
        
if (datos.size() == 9) {
                    NodoFamilia* nuevo = new NodoFamilia(
                        std::stoi(datos[0]), datos[1], datos[2], datos[3][0],
                        std::stoi(datos[4]), std::stoi(datos[5]),
                        std::stoi(datos[6]), std::stoi(datos[7]), std::stoi(datos[8])
                    );
                    
                    nodos.push_back(nuevo);
                    arbolID.insertar(nuevo);
                    
                    if (nuevo->es_lider) {
                        liderActual = nuevo;
                    }
                }
            } catch (...) {
                std::cerr << "Error procesando linea: " << linea << "\n";
            }
        }
        
        for (NodoFamilia* nodo : nodos) {
            if (nodo->id_padre == -1) {
                raizGenealogia = nodo;
            } else {
                NodoFamilia* padre = arbolID.buscar(nodo->id_padre);
                if (padre) {
                    nodo->padre = padre;
                    if (!padre->primer_hijo) {
                        padre->primer_hijo = nodo;
                    } else {
                        NodoFamilia* hermano = padre->primer_hijo;
                        while (hermano->siguiente_hermano) {
                            hermano = hermano->siguiente_hermano;
                        }
                        hermano->siguiente_hermano = nodo;
                    }
                }
            }
        }
    }
       
        void cargarContribuidores(const std::string& archivo) {
        std::string rutaAbsoluta = construirRuta(archivo);
        
        std::ifstream file(rutaAbsoluta);
        if (!file.is_open()) {
            std::cerr << "Advertencia: No se pudo abrir archivo de contribuidores\n";
            return;
        }
        
        std::string linea;
        std::getline(file, linea);
        
        while (std::getline(file, linea)) {
            try {
                std::stringstream ss(linea);
                std::string token;
                std::vector<std::string> datos;
                
                while (std::getline(ss, token, ',')) {
                    datos.push_back(token);
                }
                
                if (datos.size() >= 6) {
                    int idLider = std::stoi(datos[0]);
                    NodoFamilia* lider = arbolID.buscar(idLider);
                    
                    if (lider) {
                        lider->contribuidores.insertarOrdenado(
                            datos[1], std::stoi(datos[2]), std::stoi(datos[3]),
                            datos[4], std::stoi(datos[5])
                        );
                    }
                }
            } catch (...) {
                std::cerr << "Error procesando linea: " << linea << "\n";
            }
        }
    }

       NodoFamilia* buscarPrimerHijoVaron(NodoFamilia* nodo) {
           if (!nodo) return nullptr;
           
           Cola<NodoFamilia*> cola;
           cola.encolar(nodo);
           
           while (!cola.estaVacia()) {
               NodoFamilia* actual = cola.desencolar();
               
               if (!actual->esta_muerto && actual->genero == 'H') {
                   return actual;
               }
               
               NodoFamilia* hijo = actual->primer_hijo;
               while (hijo) {
                   cola.encolar(hijo);
                   hijo = hijo->siguiente_hermano;
               }
           }
           return nullptr;
       }
       
       NodoFamilia* encontrarSucesorMujer(NodoFamilia* lider) {
           NodoFamilia* hijo = lider->primer_hijo;
           while (hijo) {
               if (!hijo->esta_muerto && hijo->genero == 'M') {
                   return hijo;
               }
               hijo = hijo->siguiente_hermano;
           }
           
           if (lider->padre) {
               NodoFamilia* hermano = lider->padre->primer_hijo;
               while (hermano) {
                   if (hermano != lider && !hermano->esta_muerto && hermano->genero == 'M') {
                       return hermano;
                   }
                   hermano = hermano->siguiente_hermano;
               }
           }
           
           if (lider->padre && lider->padre->padre) {
               NodoFamilia* tia = lider->padre->padre->primer_hijo;
               while (tia) {
                   if (tia != lider->padre && !tia->esta_muerto && tia->genero == 'M') {
                       return tia;
                   }
                   tia = tia->siguiente_hermano;
               }
           }
           
           return nullptr;
       }
       
       NodoFamilia* encontrarSucesor(NodoFamilia* lider) {
           // Regla 1: Hijos varones vivos
           NodoFamilia* hijo = lider->primer_hijo;
           while (hijo) {
               if (!hijo->esta_muerto && hijo->genero == 'H') {
                   return hijo;
               }
               hijo = hijo->siguiente_hermano;
           }
           
           // Regla 2: Hijas mujeres y sus hijos varones
           hijo = lider->primer_hijo;
           while (hijo) {
               if (!hijo->esta_muerto && hijo->genero == 'M') {
                   NodoFamilia* nieto = hijo->primer_hijo;
                   while (nieto) {
                       if (!nieto->esta_muerto && nieto->genero == 'H') {
                           return nieto;
                       }
                       nieto = nieto->siguiente_hermano;
                   }
               }
               hijo = hijo->siguiente_hermano;
           }
           
           // Regla 3: Hermanos varones vivos o sus hijos
           if (lider->padre) {
               NodoFamilia* hermano = lider->padre->primer_hijo;
               while (hermano) {
                   if (hermano != lider && !hermano->esta_muerto) {
                       if (hermano->genero == 'H') {
                           return hermano;
                       } else {
                           NodoFamilia* sobrino = hermano->primer_hijo;
                           while (sobrino) {
                               if (!sobrino->esta_muerto && sobrino->genero == 'H') {
                                   return sobrino;
                               }
                               sobrino = sobrino->siguiente_hermano;
                           }
                       }
                   }
                   hermano = hermano->siguiente_hermano;
               }
           }
           
           // Regla 4: Tíos (hermanos del padre)
           if (lider->padre && lider->padre->padre) {
               NodoFamilia* tio = lider->padre->padre->primer_hijo;
               while (tio) {
                   if (tio != lider->padre && !tio->esta_muerto) {
                       if (tio->genero == 'H') {
                           return tio;
                       } else {
                           NodoFamilia* primo = tio->primer_hijo;
                           while (primo) {
                               if (!primo->esta_muerto && primo->genero == 'H') {
                                   return primo;
                               }
                               primo = primo->siguiente_hermano;
                           }
                       }
                   }
                   tio = tio->siguiente_hermano;
               }
           }
           
           // Regla 5: Buscar ancestros con dos hijos
           NodoFamilia* ancestro = lider;
           while (ancestro->padre) {
               ancestro = ancestro->padre;
               int numHijos = 0;
               NodoFamilia* h = ancestro->primer_hijo;
               while (h) { numHijos++; h = h->siguiente_hermano; }
               
               if (numHijos >= 2) {
                   return buscarPrimerHijoVaron(ancestro);
               }
           }
           
           return encontrarSucesorMujer(lider);
       }
       
       void cambiarLider(NodoFamilia* nuevoLider) {
           if (liderActual) {
               liderActual->es_lider = false;
               liderActual->fue_lider = true;
           }
           
           liderActual = nuevoLider;
           if (liderActual) {
               liderActual->es_lider = true;
               liderActual->fue_lider = false;
               std::cout << "Nuevo lider: " << liderActual->nombre << " " 
                         << liderActual->apellido << " (ID: " << liderActual->id << ")\n";
           }
       }
       
       void actualizarLiderazgo() {
           if (!liderActual) return;
           
           if (liderActual->edad > 70 && !liderActual->esta_muerto) {
               NodoFamilia* sucesor = encontrarSucesor(liderActual);
               if (sucesor) cambiarLider(sucesor);
               return;
           }
           
           if (liderActual->esta_muerto) {
               NodoFamilia* sucesor = encontrarSucesor(liderActual);
               if (sucesor) {
                   cambiarLider(sucesor);
               } else {
                   std::cout << "No se encontro sucesor valido\n";
               }
           }
       }
       
       void mostrarLineaSucesion() {
           if (!liderActual) {
               std::cout << "No hay lider actual\n";
               return;
           }
           
           std::cout << "\n=== Linea de Sucesion ===\n";
           std::cout << "Lider actual: " << liderActual->nombre << " " 
                     << liderActual->apellido << " (ID: " << liderActual->id 
                     << ", Edad: " << liderActual->edad << ")\n";
           
           std::cout << "\nSucesores directos:\n";
           NodoFamilia* hijo = liderActual->primer_hijo;
           while (hijo) {
               if (!hijo->esta_muerto) {
                   std::cout << "- " << hijo->nombre << " " << hijo->apellido 
                             << " (ID: " << hijo->id << ", Edad: " << hijo->edad 
                             << ", Genero: " << hijo->genero << ")\n";
               }
               hijo = hijo->siguiente_hermano;
           }
           
           if (liderActual->edad > 70) {
               std::cout << "\nADVERTENCIA: Lider mayor de 70 años\n";
           }
       }
       
       void mostrarArbolGenealogico() {
           if (!raizGenealogia) {
               std::cout << "Arbol genealogico vacio\n";
               return;
           }
           
           std::cout << "\n=== Arbol Genealogico ===\n";
           Cola<NodoFamilia*> cola;
           cola.encolar(raizGenealogia);
           
           while (!cola.estaVacia()) {
               NodoFamilia* actual = cola.desencolar();
               
               std::cout << "- " << actual->nombre << " " << actual->apellido 
                         << " (ID: " << actual->id << ")";
               
               if (actual->es_lider) std::cout << " [LIDER]";
               else if (actual->fue_lider) std::cout << " [EX-LIDER]";
               std::cout << "\n";
               
               NodoFamilia* hijo = actual->primer_hijo;
               while (hijo) {
                   cola.encolar(hijo);
                   hijo = hijo->siguiente_hermano;
               }
           }
       }
       
       void buscarMiembro(int id) {
           NodoFamilia* miembro = arbolID.buscar(id);
           if (!miembro) {
               std::cout << "Miembro no encontrado\n";
               return;
           }
           
           std::cout << "\n=== Informacion del Miembro ===\n";
           std::cout << "ID: " << miembro->id << "\n";
           std::cout << "Nombre: " << miembro->nombre << " " << miembro->apellido << "\n";
           std::cout << "Genero: " << miembro->genero << "\n";
           std::cout << "Edad: " << miembro->edad << "\n";
           std::cout << "ID Padre: " << miembro->id_padre << "\n";
           std::cout << "Estado: " << (miembro->esta_muerto ? "Muerto" : "Vivo") << "\n";
           std::cout << "Rol: ";
           if (miembro->es_lider) std::cout << "Lider actual";
           else if (miembro->fue_lider) std::cout << "Ex-lider";
           else std::cout << "Miembro comun";
           std::cout << "\n";
           
           std::cout << "\nHijos:\n";
           NodoFamilia* hijo = miembro->primer_hijo;
           while (hijo) {
               std::cout << "- " << hijo->nombre << " " << hijo->apellido 
                         << " (ID: " << hijo->id << ", Edad: " << hijo->edad 
                         << ", " << (hijo->esta_muerto ? "Muerto" : "Vivo") << ")\n";
               hijo = hijo->siguiente_hermano;
           }
       }
       
       void modificarMiembro(int id) {
           NodoFamilia* miembro = arbolID.buscar(id);
           if (!miembro) {
               std::cout << "Miembro no encontrado\n";
               return;
           }
           
           std::cout << "\nModificando: " << miembro->nombre << " " << miembro->apellido << "\n";
           
           int opcion;
           do {
               std::cout << "\n1. Cambiar nombre\n";
               std::cout << "2. Cambiar apellido\n";
               std::cout << "3. Cambiar edad\n";
               std::cout << "4. Cambiar estado\n";
               std::cout << "5. Volver\n";
               std::cout << "Opcion: ";
               std::cin >> opcion;
               
               std::string tempStr;
               int tempInt;
               
               switch (opcion) {
                   case 1:
                       std::cout << "Nuevo nombre: ";
                       std::cin >> tempStr;
                       miembro->nombre = tempStr;
                       break;
                   case 2:
                       std::cout << "Nuevo apellido: ";
                       std::cin >> tempStr;
                       miembro->apellido = tempStr;
                       break;
                   case 3:
                       std::cout << "Nueva edad: ";
                       std::cin >> tempInt;
                       miembro->edad = tempInt;
                       break;
                   case 4:
                       std::cout << "Nuevo estado (0=Vivo, 1=Muerto): ";
                       std::cin >> tempInt;
                       miembro->esta_muerto = (tempInt == 1);
                       if (miembro->es_lider) actualizarLiderazgo();
                       break;
                   case 5:
                       break;
                   default:
                       std::cout << "Opcion invalida\n";
               }
           } while (opcion != 5);
       }
       
       void mostrarContribuidoresLider() {
           if (!liderActual) {
               std::cout << "No hay lider actual\n";
               return;
           }
           
           std::cout << "\n=== Contribuidores de " << liderActual->nombre 
                     << " " << liderActual->apellido << " ===\n";
           
           liderActual->contribuidores.mostrar();
       }
       
   public:  /*
       SistemaClan() : raizGenealogia(nullptr), liderActual(nullptr) {
           std::cout << "Directorio actual: " << fs::current_path() << "\n";
           
           try {
               cargarFamilia("bin/familia.csv");
               cargarContribuidores("bin/contribuidores.csv");
               actualizarLiderazgo();
           } catch (const std::exception& e) {
               std::cerr << "Error inicial: " << e.what() << "\n";
           }
       }
       */ 
      SistemaClan() : raizGenealogia(nullptr), liderActual(nullptr) {
        std::cout << "Directorio actual: " << fs::current_path() << "\n";
        
        // Verificar existencia de archivos
        std::cout << "Ruta familia.csv: " << construirRuta("bin/familia.csv") << "\n";
        std::cout << "Ruta contribuidores.csv: " << construirRuta("bin/contribuidores.csv") << "\n";
        
        try {
            cargarFamilia("bin/familia.csv");
            std::cout << "Familia cargada. Intentando cargar contribuidores...\n";
            cargarContribuidores("bin/contribuidores.csv");
            actualizarLiderazgo();
        } catch (const std::exception& e) {
            std::cerr << "Error inicial: " << e.what() << "\n";
        }
    }

    void agregarNuevoMiembro() {
        std::cout << "\n=== Agregar Nuevo Miembro ===\n";
        
        int id, edad, id_padre;
        std::string nombre, apellido;
        char genero;
        bool esta_muerto, fue_lider, es_lider;
        
        std::cout << "ID: ";
        std::cin >> id;
        std::cout << "Nombre: ";
        std::cin >> nombre;
        std::cout << "Apellido: ";
        std::cin >> apellido;
        std::cout << "Genero (H/M): ";
        std::cin >> genero;
        std::cout << "Edad: ";
        std::cin >> edad;
        std::cout << "ID Padre (-1 si no tiene): ";
        std::cin >> id_padre;
        std::cout << "Esta muerto? (0/1): ";
        std::cin >> esta_muerto;
        std::cout << "Fue lider? (0/1): ";
        std::cin >> fue_lider;
        std::cout << "Es lider? (0/1): ";
        std::cin >> es_lider;
        
        NodoFamilia* nuevo = new NodoFamilia(
            id, nombre, apellido, toupper(genero), edad, id_padre,
            esta_muerto, fue_lider, es_lider
        );
        
        // Agregar al árbol
        arbolID.insertar(nuevo);
        
        // Establecer relaciones familiares
        if (id_padre != -1) {
            NodoFamilia* padre = arbolID.buscar(id_padre);
            if (padre) {
                nuevo->padre = padre;
                if (!padre->primer_hijo) {
                    padre->primer_hijo = nuevo;
                } else {
                    NodoFamilia* hermano = padre->primer_hijo;
                    while (hermano->siguiente_hermano) {
                        hermano = hermano->siguiente_hermano;
                    }
                    hermano->siguiente_hermano = nuevo;
                }
            }
        }
        
        if (es_lider) {
            if (liderActual) {
                liderActual->es_lider = false;
                liderActual->fue_lider = true;
            }
            liderActual = nuevo;
        }
        
        std::cout << "Miembro agregado exitosamente!\n";
    }

    void agregarNuevoContribuidor() {
        std::cout << "\n=== Agregar Nuevo Contribuidor ===\n";
        
        if (!liderActual) {
            std::cout << "No hay lider actual\n";
            return;
        }
        
        int id, edad, grado;
        std::string nombre, descripcion;
        
        std::cout << "ID del contribuidor: ";
        std::cin >> id;
        std::cout << "Nombre: ";
        std::cin.ignore();
        std::getline(std::cin, nombre);
        std::cout << "Edad: ";
        std::cin >> edad;
        std::cout << "Descripcion: ";
        std::cin.ignore();
        std::getline(std::cin, descripcion);
        std::cout << "Grado (1-10): ";
        std::cin >> grado;
        
        liderActual->contribuidores.insertarOrdenado(
            nombre, edad, id, descripcion, grado
        );
        
        std::cout << "Contribuidor agregado al lider actual (" 
                  << liderActual->nombre << " " << liderActual->apellido << ")\n";
    }

    void mostrarMenuPrincipal() {
        int opcion;
        do {
            std::cout << "\n=== Sistema del Clan Shogun ===\n";
            std::cout << "1. Mostrar linea de sucesion\n";
            std::cout << "2. Mostrar arbol genealogico\n";
            std::cout << "3. Buscar miembro\n";
            std::cout << "4. Modificar miembro\n";
            std::cout << "5. Mostrar contribuidores\n";
            std::cout << "6. Actualizar liderazgo\n";
            std::cout << "7. Agregar nuevo miembro\n";
            std::cout << "8. Agregar nuevo contribuidor\n";
            std::cout << "9. Salir\n";
            std::cout << "Opcion: ";
            std::cin >> opcion;
            
            int id;
            
            switch (opcion) {
                case 1: mostrarLineaSucesion(); break;
                case 2: mostrarArbolGenealogico(); break;
                case 3: 
                    std::cout << "ID a buscar: ";
                    std::cin >> id;
                    buscarMiembro(id);
                    break;
                case 4:
                    std::cout << "ID a modificar: ";
                    std::cin >> id;
                    modificarMiembro(id);
                    break;
                case 5: mostrarContribuidoresLider(); break;
                case 6: actualizarLiderazgo(); break;
                case 7: agregarNuevoMiembro(); break;
                case 8: agregarNuevoContribuidor(); break;
                case 9: std::cout << "Saliendo...\n"; break;
                default: std::cout << "Opcion invalida\n";
            }
        } while (opcion != 9);
    }       

   };
   
   int main() {
       SistemaClan sistema;
       sistema.mostrarMenuPrincipal();
       return 0;
   }

  