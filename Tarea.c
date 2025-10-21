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

    FILE *file = fopen(nombreArchivo, "r");
    if (file == NULL){
        printf("Error al abrir el archivo.\n");
        return -1;
    }
}

int main(){
    return 0;
}