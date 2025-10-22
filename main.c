#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <limits.h>

#define INFINITY INT_MAX //<----Se usa para definir la distancia por defecto para djkstra.

/*
Estructura de grafo G = (V,E) o digrafo D = (V,A).
Almacena punteros a cadenas de caracteres con los vértices y aristas.
*/
typedef struct {
    char* vertices;
    char* edges;
} GraphData; 

/*
Crea una estructura de un grafo respecto a sus vertices y aristas.

@param vertices: String de vertices del grafo (char*).
@param edges: String de aristas del grafo (char*).
@return graph estructura de grafo con sus propiedades (GraphData).

*/
GraphData set_graph(char vertices[], char edges[]) {
    GraphData graph;
    graph.vertices = vertices;
    graph.edges = edges;
    return graph; //---->retorna el grafo con sus propiedades.
}

/*
Comprueba si un vertice se encuentra en el grafo.

@param v: El vértice a buscar (char).
@param graph: Puntero a la estructura de datos del grafo.
@return true si el vértice existe, false en caso contrario.
*/
bool vertex_in_graph(char v, GraphData *graph) {
    const char *vertices = graph->vertices; 
    size_t len = strlen(vertices);

    for(size_t i = 0; i < len; i++) {
        if(v == vertices[i]) {
            return true;
        }
    }
    return false;
}

/*
Transforma un indice a un vertice.
Asume que los vértices son consecutivos desde 'a'.

@param i: El indice del vertice (int).
@return vertice respecto al indice (char).
*/
char index_to_vertex(int i){
    return i + 'a'; //---->retorna un vertice del indice
}

/*
Trnsforma un vertice a un indice.
Asume que los vértices son consecutivos desde 'a'.

@param c: Vertice del grafo (char).
@return indice respecto al indice (int).
*/
int vertex_to_index(char c){
    return c - 'a'; //---->retorna un indice del vertice
}

/*
Crea la matriz de adyacencia del grafo, respecto a GraphData 
y su orientación. Asigna memoria dinámicamente.

@param graph: La estructura del grafo.
@param oriented: true si es digrafo, false si es grafo no dirigido.
@return La matriz de adyacencia (int**), o NULL si falla la asignación de memoria.
*/
int** adyacent_matrix(GraphData graph, bool oriented) {
    int size = strlen(graph.vertices);

    int** matrix = (int**)malloc(size * sizeof(int*));
    if (matrix == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            for (int k = 0; k < i; k++) {
                free(matrix[k]);
            }
            free(matrix);
            return NULL;
        }
    }

    //Rellena la matriz con 0's
    for(int j=0;j<size;j++){
    for(int i=0;i<size;i++){
        matrix[i][j] = 0;
        }
    }

    //Asigna los arcos del digrafo (orientado)
    if(oriented){
        for(size_t k=0;k<strlen(graph.edges);k+=2){ //Lectura de cada arco en pares de vertices (u,v)
            int i = vertex_to_index(graph.edges[k]); // i = fuente
            int j = vertex_to_index(graph.edges[k+1]); // j = destino
            matrix[j][i] = 1; // Convención: matrix[destino][fuente]
        }
    }

    //Asigna las aristas del grafo (no orientado)
    else{
        for(size_t k=0;k<strlen(graph.edges);k+=2){ //Lectura de cada arista en pares de vertices
            int i = vertex_to_index(graph.edges[k]);
            int j = vertex_to_index(graph.edges[k+1]);
            matrix[i][j] = 1;
            matrix[j][i] = 1;
        }
    }

    return matrix; //<----retorna la matriz de adyacencia
}

/*
Imprime la matriz de adyacencia del grafo en un formato legible, 
incluyendo las etiquetas de los vértices.
*/
void print_adyacent_matrix(int **matrix, int size){

    for(int a=0;a<size;a++){
        if(a==0) printf("  ");
        printf("%c ", index_to_vertex(a));
    }
    printf("\n");
    for(int j=0;j<size;j++){
        printf("%c ", index_to_vertex(j));
        for(int i=0;i<size;i++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/*
Busca el índice del vértice no visitado con la distancia mínima (paso de extracción del mínimo de Dijkstra).
Este es el CORE de la selección en cada iteración.

@param distance: Arreglo de distancias desde el origen.
@param visited: Arreglo booleano que indica si el vértice ya fue visitado.
@param size: El número total de vértices.
@return El índice del vértice con la distancia mínima. Retorna -1 si no quedan nodos alcanzables.
*/
int get_min_dist_vertex(int* distance, bool* visited, int size) {
    int min_dist = INT_MAX;
    int min_index = -1; // -1 representa "no encontrado" o "Null"

    for (int v = 0; v < size; v++) {
        if (visited[v] == false && distance[v] < min_dist) {
            min_dist = distance[v];
            min_index = v;
        }
    }
    return min_index;
}

/*
Implementación del algoritmo de Dijkstra para encontrar la ruta más corta 
desde un vértice fuente a todos los demás. Asume peso de arista = 1 (por la matriz binaria).

@param matrix: La matriz de adyacencia del grafo (int**).
@param size: El número total de vértices.
@param s_char: El vértice fuente (char).
@param distance: Array de salida para almacenar las distancias mínimas.
@param visited: Array de salida para rastrear vértices visitados.
@param predecessor: Array de salida para almacenar los predecesores en la ruta más corta.
*/
void dijkstra(int** matrix, int size, char s_char, int* distance, bool* visited, int* predecessor) {

    int s_idx = vertex_to_index(s_char); // Índice del nodo fuente 's'

    // Inicialización: Distancia a infinito, visitado a falso, predecesor a -1
    for (int v_idx = 0; v_idx < size; v_idx++) {
        distance[v_idx] = INFINITY; // infinito (INT_MAX)
        visited[v_idx] = false;       // Falso
        predecessor[v_idx] = -1;          // Null (usamos -1)
    }

    // Distancia del nodo fuente a sí mismo es 0
    distance[s_idx] = 0;

    // Bucle principal: se ejecuta 'size' veces (o hasta que todos los alcanzables sean visitados)
    for (int count = 0; count < size; count++) {

        // 1. Extraer el nodo no visitado con la distancia mínima
        int u_idx = get_min_dist_vertex(distance, visited, size);

        // Si u_idx es -1, los nodos restantes no son alcanzables desde s
        if (u_idx == -1) {
            break;
        }

        // 2. Marcar como visitado
        visited[u_idx] = true;

        // 3. Relajación: para todo vecino 'v' de 'u'
        for (int v_idx = 0; v_idx < size; v_idx++) {

            // weight(u, v) se obtiene de la matriz: matrix[destino][fuente]
            int weight_uv = matrix[v_idx][u_idx];

            // La condición "v in N+(u)" es "weight_uv > 0" (hay una arista)
            if (weight_uv > 0) {

                // Si la distancia[u] es infinito, no podemos relajar desde él (evita desbordamiento)
                if (distance[u_idx] == INFINITY) {
                    continue;
                }

                // Relajación: if (distance[v] > distance[u] + weight(u, v))
                if (distance[v_idx] > distance[u_idx] + weight_uv) {

                    // Actualizar distancia y predecesor
                    distance[v_idx] = distance[u_idx] + weight_uv;
                    predecessor[v_idx] = u_idx;
                }
            }
        }
    }
}

/*
Reconstruye e imprime el camino más corto desde el origen hasta el destino 
utilizando el arreglo de predecesores generado por Dijkstra.

@param predecessor: Arreglo de predecesores (índices de los vértices).
@param distance: Arreglo de distancias mínimas.
@param size: Número total de vértices.
@param source_char: Vértice de inicio (char).
@param dest_char: Vértice de destino (char).
*/
void reconstruct_path(int* predecessor, int* distance, int size, char source_char, char dest_char) {
    int s_idx = vertex_to_index(source_char);
    int d_idx = vertex_to_index(dest_char);

    // Comprobar si se encontró un camino
    if (distance[d_idx] == INFINITY) {
        printf("No se encontro un camino desde %c hasta %c.\n", source_char, dest_char);
        return;
    }

    printf("\n--- Resultado de Dijkstra ---\n");
    printf("Camino mas corto desde %c hasta %c (Costo: %d):\n", 
        source_char, dest_char, distance[d_idx]);

    int* path_indices = (int*)malloc(size * sizeof(int));
    if (path_indices == NULL) {
        printf("Error de memoria al reconstruir el camino.\n");
        return;
    }

    int current_idx = d_idx;
    int path_len = 0;

    // Trazar el camino hacia atrás desde el destino
    while (current_idx != -1 && path_len < size) {
        path_indices[path_len++] = current_idx;
        if (current_idx == s_idx) {
            break; // Llegamos al inicio
        }
        current_idx = predecessor[current_idx];
    }

    // Imprimir el camino en el orden correcto (al revés de como lo trazamos)
    for (int i = path_len - 1; i >= 0; i--) {
        printf("%c", index_to_vertex(path_indices[i]));
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");

    free(path_indices);
}

/*
Lectura del archivo de texto que contiene al grafo (tipo, vértices y aristas).
Asigna memoria dinámicamente para las cadenas de vértices y aristas 
y para la matriz de adyacencia.

@param nameFile: Nombre del archivo a leer.
@param out_graph: Puntero a la estructura GraphData que se rellenará.
@return La matriz de adyacencia (int**), o NULL en caso de error.
*/
int** readFile(const char *nameFile, GraphData *out_graph) {
    //Comprueba que el archivo sea valido
    FILE *file = fopen(nameFile, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo '%s'.\n", nameFile);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    char type[10];
    bool oriented = false;

    // 1. Lee el tipo de grafo (Línea 1)
    if (fgets(buffer, sizeof(buffer), file) == NULL || sscanf(buffer, "%s", type) != 1) {
        printf("Error: Archivo vacio o formato invalido (tipo de grafo).\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (strcmp(type, "digraph") == 0) {
        oriented = true;
        printf("Grafo dirigido (digraph) detectado.\n");
    } else if (strcmp(type, "graph") == 0) {
        oriented = false;
        printf("Grafo no dirigido (graph) detectado.\n");
    } else {
        printf("Error: Tipo de grafo '%s' no reconocido.\n", type);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // 2. Lee los vértices (Línea 2)
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Error: No se encontro la linea de vertices.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    size_t vertices_len = strcspn(buffer, "\n");
    if (vertices_len == 0) {
        printf("Error: La lista de vertices esta vacia.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // Asignación dinámica: ESTA MEMORIA DEBE SER LIBERADA EN main
    char* vertices_str = (char*)malloc(vertices_len + 1);
    if (vertices_str == NULL) { /* ... error handling ... */ exit(EXIT_FAILURE); }
    strncpy(vertices_str, buffer, vertices_len);
    vertices_str[vertices_len] = '\0';


    // 3. Lee las aristas (Línea 3)
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Error: No se encontro la linea de aristas.\n");
        free(vertices_str);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    size_t edges_len = strcspn(buffer, "\n");
    if (edges_len % 2 != 0) {
        printf("Error: La cadena de aristas tiene una longitud impar.\n");
        free(vertices_str);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // Asignación dinámica: ESTA MEMORIA DEBE SER LIBERADA EN main
    char* edges_str = (char*)malloc(edges_len + 1);
    if (edges_str == NULL) { /* ... error handling ... */ exit(EXIT_FAILURE); }
    strncpy(edges_str, buffer, edges_len);
    edges_str[edges_len] = '\0';

    fclose(file);

    // 4. Rellena la estructura GraphData de salida
    out_graph->vertices = vertices_str;
    out_graph->edges = edges_str;

    printf("Vertices: %s (total: %zu)\n", out_graph->vertices, strlen(out_graph->vertices));
    printf("Aristas: %s (count: %zu)\n", out_graph->edges, strlen(out_graph->edges)/2);

    // 5. Genera y devolvuelve la matriz de adyacencia
    int **matrix = adyacent_matrix(*out_graph, oriented);

    return matrix; //---->retorna la matriz de adyacencia del grafo
}

int main(int argc, char *argv[]){
    //Comprueba la cantidad de argumentos de entrada
    if (argc != 5){
        printf("Error, la cantidad de parametros debe ser 5.\n");
        return -1;
    }
    //Valida la entrada
    if(strcmp(argv[1], "path") == 0){
        const char *filename = argv[4];

        GraphData my_graph;
        my_graph.vertices = NULL;
        my_graph.edges = NULL;

        //Crea la matriz de adyacencia y la estructura del grafo
        int **matrix = readFile(filename, &my_graph); 

        int size=0;
        if(my_graph.vertices != NULL){
            size = strlen(my_graph.vertices);
        }

        // Validación de vértices
        if (strlen(argv[2]) == 1 && strlen(argv[3]) == 1 && 
            vertex_in_graph(argv[2][0], &my_graph) && vertex_in_graph(argv[3][0], &my_graph)) {

            if (matrix != NULL) {
                printf("\n--- Matriz de Adyacencia ---\n");
                print_adyacent_matrix(matrix, size);

                // Asignación de memoria para los arrays de Dijkstra
                int* distance = (int*)malloc(size * sizeof(int));
                bool* visited = (bool*)malloc(size * sizeof(bool));
                int* predecessor = (int*)malloc(size * sizeof(int));

                if (distance == NULL || visited == NULL || predecessor == NULL) {
                    printf("Error: No se pudo asignar memoria para los arreglos de Dijkstra.\n");
                    // Manejo de errores de memoria (liberación parcial)
                    free(distance); free(visited); free(predecessor);
                    for (int i = 0; i < size; i++) free(matrix[i]);
                    free(matrix);
                    free(my_graph.vertices); free(my_graph.edges);
                    return -1;
                }

                char source_node = argv[2][0];
                char dest_node = argv[3][0];

                // Ejecución y reconstrucción
                dijkstra(matrix, size, source_node, distance, visited, predecessor);
                reconstruct_path(predecessor, distance, size, source_node, dest_node);

                // Liberación de memoria de los arrays de Dijkstra
                free(distance);
                free(visited);
                free(predecessor);

                // Liberación de la matriz
                for (int i = 0; i < size; i++) {
                    free(matrix[i]);
                }
                free(matrix);

                // Liberación de los strings de GraphData
                free(my_graph.vertices);
                free(my_graph.edges);

                return 0; // Éxito
            }
        }
 
        // Manejo de errores de validación
        printf("Error, vertices no validos o no existen en el grafo.\n");
        if (matrix != NULL) {
            for(int i = 0; i < size; i++) {
                free(matrix[i]);
            }
            free(matrix);
        }
        // Liberación de datos de my_graph en caso de error de validación
        free(my_graph.vertices);
        free(my_graph.edges);

        return -1;
    }
    return -1;
}