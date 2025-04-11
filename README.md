# proyecto_shogun
Descripción del Problema
Se necesita digitalizar los registros genealógicos de un clan shogun, representando las relaciones familiares como un árbol binario balanceado (AVL) y gestionando:

Línea de sucesión automática según reglas específicas

Contribuciones de miembros externos al líder actual

Operaciones CRUD (Crear, Leer, Actualizar, Eliminar) sobre los miembros

>>¿Por qué usar un Árbol AVL?
Requerimiento	                         Solución AVL	                                Beneficio
Búsquedas rápidas por ID	        Balanceo automático	                        Tiempo O(log n) garantizado
Mantener orden jerárquico	        Estructura de árbol	                    Representación natural de relaciones padre-hijo
Actualizaciones frecuentes	        Rotaciones balanceadas	                Mantiene eficiencia tras inserciones/eliminaciones

>>Estructuras Clave y su Justificación
Componente	                        Estructura                                           	Razón
Árbol Genealógico	        Árbol AVL (por ID) + Listas enlazadas (hijos/hermanos)	        Búsqueda eficiente + Representación jerárquica
Contribuidores	            Lista enlazada ordenada (por grado y edad)                    	Inserción/consulta rápida manteniendo orden


Flujo del Programa
Inicialización:

Carga datos desde familia.csv y contribuidores.csv

Construye el árbol AVL y las relaciones familiares

Menú Principal:

1. Mostrar sucesión
2. Mostrar árbol
3. Buscar miembro
4. Modificar miembro
5. Añadir miembro
6. Añadir contribuidor
Gestión de Memoria:

Destructores recursivos liberan nodos del AVL y listas de contribuidores

Ejemplo de Ejecución

# Compilación
g++ -std=c++17 -Isrc src/*.cpp -o bin/proyecto_shogun

# Ejecución
bin/proyecto_shogun
Salida:

=== Linea de Sucesion ===
Lider actual: Jean Mendoza (ID: 2, Edad: 44)

Sucesores directos:
- Arturo Mendoza (ID: 9, Edad: 4, Genero: H)

=== Contribuidores ===
1,101,Miguel Santos,45,Contador,9
1,102,Yamal Kansuke,60,Amigo,3
Posibles Extensiones
Persistencia: Guardar cambios automáticamente en CSV

Visualización: Generar gráficos del árbol con Graphviz

Conclusión
Este proyecto demuestra cómo un árbol AVL es ideal para:

Modelar jerarquías complejas (familias)

Mantener operaciones eficientes (log n)

Aplicar reglas de negocio (sucesión) mediante recorridos

El diseño separa claramente:

Almacenamiento (AVL + listas)

Lógica (reglas de sucesión)

Interfaz (menú interactivo)

