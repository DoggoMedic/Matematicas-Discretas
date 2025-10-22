#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <limits.h>

//int distancia[MAX_VERTICES];
//int predecesor[MAX_VERTICES];
//int visitado[MAX_VERTICES];

typedef struct {
    char* vertices;
    char* edges;
} GraphData; 

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

char index_to_vertex(int i){
    return i + 'a';
}

int vertex_to_index(char c){
    return c - 'a';
}

GraphData set_graph(char vertices[], char edges[]) {
    GraphData graph;
    graph.vertices = vertices;
    graph.edges = edges;
    return graph;
}

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

    //Fill matrix
    for(int j=0;j<size;j++){
        for(int i=0;i<size;i++){
            matrix[i][j] = 0;
        }
    }

    if(oriented){
        for(size_t k=0;k<strlen(graph.edges);k+=2){
            int i = vertex_to_index(graph.edges[k]);
            int j = vertex_to_index(graph.edges[k+1]);
            matrix[j][i] = 1;
        }
    }

    else{
        for(size_t k=0;k<strlen(graph.edges);k+=2){
            int i = vertex_to_index(graph.edges[k]);
            int j = vertex_to_index(graph.edges[k+1]);
            matrix[i][j] = 1;
            matrix[j][i] = 1;
        }
    }

    return matrix;
}

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

char *find_path(){
    return "abcd";
}

int get_min_dist_vertex(int* distancia, bool* visto, int size) {
    int min_dist = INT_MAX;
    int min_index = -1; // -1 representa "no encontrado" o "Null"

    for (int v = 0; v < size; v++) {
        if (visto[v] == false && distancia[v] < min_dist) {
            min_dist = distancia[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int** matrix, int size, char s_char, int* distancia, bool* visto, int* padre) {
    
    int s_idx = vertex_to_index(s_char); // Índice del nodo fuente 's'

    // Líneas 1-4: Inicialización
    for (int v_idx = 0; v_idx < size; v_idx++) {
        distancia[v_idx] = INT_MAX; // infinito
        visto[v_idx] = false;       // Falso
        padre[v_idx] = -1;          // Null (usamos -1 como Null)
    }

    // Línea 5: distancia[s] <- 0
    distancia[s_idx] = 0;

    // Línea 6: while (existe v no visto)
    // Se puede implementar como un bucle que corre 'size' veces
    for (int count = 0; count < size; count++) {
        
        // Línea 7: u <- coger el mínimo del vector distancia y que no esté visto
        int u_idx = get_min_dist_vertex(distancia, visto, size);

        // Si u_idx es -1, los nodos restantes no son alcanzables desde s
        if (u_idx == -1) {
            break;
        }

        // Línea 8: visto[u] <- Verdadero
        visto[u_idx] = true;

        // Línea 9: forall v in N+(u) do (para todo vecino 'v' de 'u')
        // Iteramos por todos los vértices 'v' y comprobamos si son vecinos de 'u'
        for (int v_idx = 0; v_idx < size; v_idx++) {
            
            // peso(u, v) se obtiene de la matriz.
            // Recordar: matrix[destino][fuente] -> matrix[v_idx][u_idx]
            int peso_uv = matrix[v_idx][u_idx];

            // La condición "v in N+(u)" es "peso_uv > 0" (hay una arista)
            if (peso_uv > 0) {
                
                // Si la distancia[u] es infinito, no podemos relajar desde él
                if (distancia[u_idx] == INT_MAX) {
                    continue;
                }

                // Línea 10: if (distancia[v] > distancia[u] + peso(u, v))
                if (distancia[v_idx] > distancia[u_idx] + peso_uv) {
                    
                    // Línea 11: distancia[v] <- distancia[u] + peso(u, v)
                    distancia[v_idx] = distancia[u_idx] + peso_uv;
                    
                    // Línea 12: padre[v] <- u
                    padre[v_idx] = u_idx;
                }
            }
        }
    }
}

void reconstruct_path(int* padre, int* distancia, int size, char source_char, char dest_char) {
    int s_idx = vertex_to_index(source_char);
    int d_idx = vertex_to_index(dest_char);

    // Comprobar si se encontró un camino
    if (distancia[d_idx] == INT_MAX) {
        printf("No se encontro un camino desde %c hasta %c.\n", source_char, dest_char);
        return;
    }

    printf("\n--- Resultado de Dijkstra ---\n");
    printf("Camino mas corto desde %c hasta %c (Costo: %d):\n", 
           source_char, dest_char, distancia[d_idx]);
    
    // Asignar memoria para el string del camino
    // El tamaño máximo es 'size' vértices + (size-1) " -> " + '\0'
    // Simplificado: un buffer temporal para los índices
    int* path_indices = (int*)malloc(size * sizeof(int));
    if (path_indices == NULL) {
        printf("Error de memoria al reconstruir el camino.\n");
        return;
    }

    int current_idx = d_idx;
    int path_len = 0;

    // Trazar el camino hacia atrás desde el destino
    while (current_idx != -1) {
        path_indices[path_len++] = current_idx;
        if (current_idx == s_idx) {
            break; // Llegamos al inicio
        }
        current_idx = padre[current_idx];
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

int** readFile(const char *nameFile, GraphData *out_graph) {
    FILE *file = fopen(nameFile, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo '%s'.\n", nameFile);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    char type[10];
    bool oriented = false;

    // --- 1. Leer el tipo de grafo (Línea 1) ---
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
    
    // --- 2. Leer los vértices (Línea 2) ---
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


    // --- 3. Leer las aristas (Línea 3) ---
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

    // --- 4. Rellenar la estructura GraphData de salida ---
    out_graph->vertices = vertices_str;
    out_graph->edges = edges_str;     
    
    printf("Vertices: %s (total: %zu)\n", out_graph->vertices, strlen(out_graph->vertices));
    printf("Aristas: %s (count: %zu)\n", out_graph->edges, strlen(out_graph->edges)/2);

    // --- 5. Generar y devolver la matriz ---
    int **matrix = adyacent_matrix(*out_graph, oriented);
    
    return matrix; // Devuelve la matriz (memoria asignada con malloc).
}

int main(int argc, char *argv[]){  //int argc, char *argv[]
    if (argc != 5){
        printf("Error, la cantidad de parametros debe ser 5.");
        return -1;
    }
    if(strcmp(argv[1], "path") == 0){
        const char *filename = argv[4];
        
        GraphData my_graph;
        my_graph.vertices = NULL;
        my_graph.edges = NULL;
        
        int **matrix = readFile(filename, &my_graph); 

        int size=0;
        if(my_graph.vertices != NULL){
            size = strlen(my_graph.vertices);
        }



        // Corregido: Accede al carácter en la posición [0] de la cadena
        if (strlen(argv[2]) == 1 && strlen(argv[3]) == 1 && 
            vertex_in_graph(argv[2][0], &my_graph) && vertex_in_graph(argv[3][0], &my_graph)) {
        
            if (matrix != NULL) {
                int size = strlen(my_graph.vertices);
                printf("\n--- Matriz de Adyacencia (Objeto Persistente) ---\n");
                print_adyacent_matrix(matrix, size);
                int* distancia = (int*)malloc(size * sizeof(int));
                bool* visto = (bool*)malloc(size * sizeof(bool));
                int* padre = (int*)malloc(size * sizeof(int));

                if (distancia == NULL || visto == NULL || padre == NULL) {
                    printf("Error: No se pudo asignar memoria para los arreglos de Dijkstra.\n");
                    // Liberar todo antes de salir
                    free(distancia);
                    free(visto);
                    free(padre);
                    for (int i = 0; i < size; i++) free(matrix[i]);
                    free(matrix);
                    free(my_graph.vertices);
                    free(my_graph.edges);
                    return -1;
                }

                char source_node = argv[2][0];
                char dest_node = argv[3][0];

                dijkstra(matrix, size, source_node, distancia, visto, padre);
                reconstruct_path(padre, distancia, size, source_node, dest_node);
                free(distancia);
                free(visto);
                free(padre);
                
                // 1. Liberar la matriz (int**) que fue devuelta por readFile
                for (int i = 0; i < size; i++) {
                    free(matrix[i]);
                }
                free(matrix);

                // 2. Liberar los strings (char*) que están DENTRO de my_graph
                // Estos fueron asignados con malloc en readFile.
                free(my_graph.vertices);
                free(my_graph.edges);
            }
            find_path();
            return 0;
        }
        printf("Error, vertices no validos.\n");
        if (matrix != NULL) {
            for(int i = 0; i < size; i++) {
                free(matrix[i]);
            }
            free(my_graph.vertices);
            free(my_graph.edges);
        return -1;
    }
    return -1;
}
