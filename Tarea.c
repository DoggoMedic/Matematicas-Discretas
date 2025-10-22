#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAX_VERTICES 12
#define INFINITY __INT_MAX__

int distancia[MAX_VERTICES];
int predecesor[MAX_VERTICES];
int visitado[MAX_VERTICES];

int nodoAIndice (char c){
    return c - 'a';
}

char indiceANodo (int i){
    return i + 'a';
}

int leerArchivo(int matrizAdyacente[MAX_VERTICES][MAX_VERTICES], int *numNodos, const char *nombreArchivo, int orientacion){

    for (int i=0; i<MAX_VERTICES; i++){
        for(int j=0; j<MAX_VERTICES; j++){
            matrizAdyacente[i][j]=0;
        }
    }
    *numNodos = 0;

    int tempMatriz[MAX_VERTICES][MAX_VERTICES]={0};
    int masx_indice = -1;

    FILE *file = fopen(nombreArchivo, "r");
    if (file == NULL){
        printf("Error al abrir el archivo.\n");
        return 0;
    }
    char buffer[256];
    char nodo1_c, nodo2_c;
    int arista_index = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        if (buffer[0] == '\n' || buffer[0] == '#') continue;

        if (sscanf(buffer, " %c %c", &nodo1_c, &nodo2_c) == 2){
            int u = nodoAIndice(nodo1_c);
            int v = nodoAIndice(nodo2_c);

            if (u < 0 || u >= MAX_VERTICES || v < 0 || v >= MAX_VERTICES) {
                printf("Advertencia: Nodo '%c' o '%c' excede el limite de MAX_VERTICES (a-%c) y fue ignorado.\n", nodo1_c, nodo2_c, indice_a_nodo(MAX_VERTICES - 1));
                continue;
            }

            tempMatriz[u][v] = 1;
            tempMatriz[v][u] = 1;

            if (u > masx_indice) masx_indice = u;
            if (v > masx_indice) masx_indice = v;
        }
    }
    if (masx_indice == -1){
        printf("Error: No se encontraron aristas validas en el archivo.\n");
        fclose(file);
        return 0;
    }
    *numNodos = masx_indice + 1;
    fseek(file, 0, SEEK_SET);
    arista_index = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        if (buffer[0] == '\n' || buffer[0] == '#') continue;

        if (sscanf(buffer, " %c %c", &nodo1_c, &nodo2_c) == 2){
            int u = nodoAIndice(nodo1_c);
            int v = nodoAIndice(nodo2_c);

            if (u >= *numNodos || v >= *numNodos || u < 0 || v < 0) continue;

            if (orientacion == 0){
                matrizAdyacente[u][v] = 1;
                matrizAdyacente[v][u] = 1;
            } else {
                if (orientacion == 1){
                    if (u < v) matrizAdyacente[u][v] = 1; else matrizAdyacente[v][u] = 1;
                } else if (orientacion == 2){
                    if (u > v) matrizAdyacente[u][v] = 1; else matrizAdyacente[v][u] = 1;
                } else {
                    if (arista_index % 2 == 0) matrizAdyacente[u][v] = 1; else matrizAdyacente[v][u] = 1;
                }
            }
            arista_index++;
        }
    }
    fclose(file);
    return 1;
}

int main(){
    return 0;
}