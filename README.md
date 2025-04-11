# proyecto_shogun
Descripción del Problema
Se necesita digitalizar los registros genealógicos de un clan shogun, representando las relaciones familiares como un árbol binario balanceado (AVL) y gestionando:

Línea de sucesión automática según reglas específicas

Contribuciones de miembros externos al líder actual

Operaciones CRUD (Crear, Leer, Actualizar, Eliminar) sobre los miembros

¿Por qué usar un Árbol AVL?
Requerimiento	                Solución AVL	                    Beneficio
Búsquedas rápidas por ID	Balanceo automático	Tiempo O(log n) garantizado
Mantener orden jerárquico	Estructura de árbol	Representación natural de relaciones padre-hijo
Actualizaciones frecuentes	Rotaciones balanceadas	Mantiene eficiencia tras inserciones/eliminaciones
Estructuras Clave y su Justificación
Componente	Estructura	Razón
Árbol Genealógico	Árbol AVL (por ID) + Listas enlazadas (hijos/hermanos)	Búsqueda eficiente + Representación jerárquica
Contribuidores	Lista enlazada ordenada (por grado y edad)	Inserción/consulta rápida manteniendo orden
Sucesión	Algoritmos BFS/DFS	Recorrido óptimo para aplicar reglas hereditarias
Algoritmos Principales
Búsqueda de Sucesor:

python
Copy
def encontrar_sucesor(lider):
    if lider.edad > 70:
        return primer_hijo_varon_vivo(lider)
    elif lider.esta_muerto:
        return aplicar_reglas_herencia(lider)  # Reglas 1-6
Complejidad: O(n) (peor caso, al recorrer todo el árbol)

Inserción en AVL:

cpp
Copy
Nodo* insertar(Nodo* nodo, MiembroClan dato) {
    // Inserción BST normal + balanceo con rotaciones
    if (balance > 1) rotarDerecha(nodo);
    if (balance < -1) rotarIzquierda(nodo);
}
Complejidad: O(log n)

Flujo del Programa
Inicialización:

Carga datos desde familia.csv y contribuidores.csv

Construye el árbol AVL y las relaciones familiares

Menú Principal:

plaintext
Copy
1. Mostrar sucesión
2. Mostrar árbol
3. Buscar miembro
4. Modificar miembro
5. Añadir miembro
6. Añadir contribuidor
Gestión de Memoria:

Destructores recursivos liberan nodos del AVL y listas de contribuidores

Ejemplo de Ejecución
bash
Copy
# Compilación
g++ -std=c++17 -Isrc src/*.cpp -o bin/proyecto_shogun

# Ejecución
bin/proyecto_shogun
Salida:

Copy
=== Línea de Sucesión ===
Líder actual: Takeshi Shimura (ID: 1, Edad: 75)
Sucesor directo: Haruto Shimura (ID: 2)

=== Contribuidores ===
1. Yamamoto Kansuke (Grado 10): Estratega militar
2. Takeda Nobushige (Grado 9): General del ejército
Posibles Extensiones
Persistencia: Guardar cambios automáticamente en CSV

Visualización: Generar gráficos del árbol con Graphviz

Reglas avanzadas: Incorporar matrimonios o adopciones

Diferencia clave vs. Grafos
Característica	Grafo (Ciudades)	Árbol AVL (Clan)
Estructura	Nodos conexos no jerárquicos	Jerarquía padre-hijo
Búsqueda	BFS/DFS para caminos	Búsqueda por ID (O(log n))
Balanceo	No aplica	Rotaciones AVL críticas
Relaciones	Simétricas (no dirigidas)	Dirigidas (padre → hijo)
Conclusión
Este proyecto demuestra cómo un árbol AVL es ideal para:

Modelar jerarquías complejas (familias)

Mantener operaciones eficientes (log n)

Aplicar reglas de negocio (sucesión) mediante recorridos

El diseño separa claramente:

Almacenamiento (AVL + listas)

Lógica (reglas de sucesión)

Interfaz (menú interactivo)

