// Grafo ponderado não direcional por matriz de adjacencias

#include <stdio.h>
#include <stdlib.h>

#define sem_aresta -1

typedef float Peso;

typedef int bool;
#define true 1
#define false 0

typedef struct grafo {
    int numVertices;
    int numArestas;
    Peso** matriz;
} Grafo;

Grafo* inicializa_grafo(int vertices) {
    if (vertices < 1) return NULL;
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->numArestas = 0;
    g->numVertices = vertices;

    g->matriz = (Peso**) malloc(vertices * sizeof(Peso*));

    for (int i = 0; i < vertices; i++) {
        g->matriz[i] = (Peso*)malloc(vertices * sizeof(Peso));
        for (int j = 0; j < vertices; j++) {
            g->matriz[i][j] = sem_aresta;
        }
    }

    return g;
}

bool inserir_aresta(Grafo* g, int origem, int destino, Peso valor) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices || origem == destino) return false;
    if (g->matriz[origem][destino] == sem_aresta) {
        if (valor != sem_aresta) {
            g->matriz[origem][destino] = valor;
            g->matriz[destino][origem] = valor;
            g->numArestas++;
    } else {
            g->matriz[origem][destino] = valor;
            g->matriz[destino][origem] = valor;
    }
    if (valor == sem_aresta) g->numArestas--;
    }
    return true;
}

bool remove_aresta(Grafo* g, int origem, int destino) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices || g->matriz[origem][destino] == sem_aresta) return false;
    g->matriz[origem][destino] = sem_aresta;
    g->matriz[destino][origem] = sem_aresta;
    g->numArestas--;
    return true;
}

void exibe_grafo(Grafo* g) {
    if (!g) return;
    printf("Imprimindo grafo (vertices: %i; arestas %i).\n", g->numVertices, g->numArestas);
    for (int i = 0; i < g->numVertices; i++) printf("\t%5i", i);
    printf("\n");
    for (int i = 0; i < g->numVertices; i++) {
        printf("%i", i);
        for (int j = 0; j < g->numVertices; j++) {
            if (g->matriz[i][j] == sem_aresta) {
                printf("\t      ");
            } else {
                printf("\t%5.2f", g->matriz[i][j]);
            }
        }
        printf("\n");
    }
}

bool libera_grafo(Grafo* g) {
    if (g == NULL) return false;

    for (int i = 0; i < g->numVertices; i++) {
        free(g->matriz[i]);
    }

    free(g->matriz);

    g->numArestas = 0;
    g->numVertices = 0;
    g->matriz = NULL;
    return true;
}

bool aresta_existe(Grafo* g, int origem, int destino) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices || g->matriz[origem][destino] == sem_aresta) return false;
    return true;
}

int numero_vertices(Grafo* g) {
    if (g != NULL) return g->numVertices;
    return -1;
}

int numero_aresta(Grafo* g) {
    if (g != NULL) return g->numArestas;
    return -1;
}

bool possui_vizinho(Grafo* g, int nodo) {
    if (!g || nodo < 0 || nodo >= g->numVertices) return false;
    for (int i = 0; i < g->numVertices; i++) {
        if (g->matriz[nodo][i] != sem_aresta) return true;
    }
    return false;
}

int retorna_grau(Grafo* g, int nodo) {
    if (!g || nodo < 0 || nodo >= g->numVertices) return -1;
    int grau = 0;
    for (int i = 0; i < g->numVertices; i++) {
        if (g->matriz[nodo][i] != sem_aresta) grau++; 
    }

    return grau;
}

void visitaProfundidade(Grafo* g, int atual, bool* visitado, int anterior) {
    printf("Visitando vertice: %3i (anterior: %3i)\n", atual, anterior);
    visitado[atual] = true;
    for (int x = 0; x<g->numVertices; x++) {
        if (g->matriz[atual][x] != sem_aresta && !visitado[x]) {
            visitaProfundidade(g, x, visitado, atual);
        }
    }
}

void buscaEmProfundidade(Grafo* g) {
    if (!g || g->numVertices < 1) return;
    bool* visitado = (bool*) malloc(sizeof(bool)*g->numVertices);
    for (int x = 0; x < g->numVertices; x++) visitado[x] = false;
    for (int x = 0; x < g->numVertices; x++) {
        if (!visitado[x]) {
            visitaProfundidade(g, x, visitado, -1);
        }
    }
    free(visitado);
}

int main() {
    Grafo *g = inicializa_grafo(5);
    inserir_aresta(g, 1, 2, 5);
    inserir_aresta(g, 3, 0, 2);
    inserir_aresta(g, 0, 2, 6);
    exibe_grafo(g);
    printf("\n");
    buscaEmProfundidade(g);
    printf("\n");
    remove_aresta(g, 1, 2);
    exibe_grafo(g);
    printf("\n");
    buscaEmProfundidade(g);
    printf("\n");
    printf("%d", numero_aresta(g));
    printf("\n");
    printf("%d", numero_vertices(g));
    printf("\n");
    printf("%i", possui_vizinho(g, 2));
    printf("\n");
    printf("%d", retorna_grau(g, 0));
    printf("\n");
    libera_grafo(g);
    exibe_grafo(g);
    printf("\n");
    buscaEmProfundidade(g);

    return 1;
}