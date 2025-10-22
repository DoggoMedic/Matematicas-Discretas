#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>

//int distancia[MAX_VERTICES];
//int predecesor[MAX_VERTICES];
//int visitado[MAX_VERTICES];


typedef struct {
    char* vertices;
    char* edges;
} GraphData; 

char index_to_vertix(int i){
    return i + 'a';
}

int vertix_to_index(char c){
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
            int i = vertix_to_index(graph.edges[k]);
            int j = vertix_to_index(graph.edges[k+1]);
            matrix[j][i] = 1;
        }
    }

    else{
        for(size_t k=0;k<strlen(graph.edges);k+=2){
            int i = vertix_to_index(graph.edges[k]);
            int j = vertix_to_index(graph.edges[k+1]);
            matrix[i][j] = 1;
            matrix[j][i] = 1;
        }
    }

    return matrix;
}

void print_adyacent_matrix(int **matrix, int size){

    for(int a=0;a<size;a++){
        if(a==0) printf("  ");
        printf("%c ", index_to_vertix(a));
    }
    printf("\n");
    for(int j=0;j<size;j++){
        printf("%c ", index_to_vertix(j));
        for(int i=0;i<size;i++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
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

int main(){  //int argc, char *argv[]
const char *test_filename = "../Grafos no orientados/Planar.txt";
    
    GraphData my_graph;
    
    int **matrix = readFile(test_filename, &my_graph); 
    
    if (matrix != NULL) {
        int size = strlen(my_graph.vertices);
        printf("\n--- Matriz de Adyacencia (Objeto Persistente) ---\n");
        print_adyacent_matrix(matrix, size);
        
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
    
    return 0;
}
