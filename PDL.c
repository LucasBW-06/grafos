// Grafo ponderado direcional por listas de adjacencias

#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

typedef struct aux{
    int vertice;
    float peso;
    struct aux* prox;
} ElemLista, *PONT;


typedef struct {
    int numVertices;
    int numArestas;
    ElemLista** A;
} Grafo;

Grafo* inicializa_grafo(int vertices) {
    if (vertices < 1) return NULL;
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->numArestas = 0;
    g->numVertices = vertices;

    g->A = (ElemLista**) malloc(vertices * sizeof(ElemLista*));

    for (int i = 0; i < vertices; i++) {
        g->A[i] = NULL;
    }

    return g;
}

bool inserir_aresta(Grafo* g, int origem, int destino, float peso) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices) return false;
    ElemLista *novo, *ant = NULL;
    ElemLista* atual = g->A[origem];
    while (atual && atual->vertice < destino) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual && atual->vertice==destino) return false;
    novo = (ElemLista*)malloc(sizeof(ElemLista));
    novo->vertice = destino;
    novo->prox = atual;
    novo->peso = peso;
    if (ant) {
        ant->prox = novo;
    } else {
        g->A[origem] = novo;
    }
    g->numArestas++;
    return true;
}

bool remove_aresta(Grafo* g, int origem, int destino) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices) return false;
    ElemLista *ant = NULL;
    ElemLista* atual = g->A[origem];
    while (atual && atual->vertice < destino) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual && atual->vertice == destino) {
        if (ant) {
            ant->prox = atual->prox;
        } else {
            g->A[origem] = atual->prox;
        }
        free(atual);
        g->numArestas--;
        return true;
    }
    return false;
}

void exibe_grafo(Grafo* g) {
    if (!g) return;
    printf("Imprimindo grafo (vertices: %i; arestas %i).\n", g->numVertices, g->numArestas);
    ElemLista* atual;
    for (int i = 0; i < g->numVertices; i++) {
        printf("[%2i]", i);
        atual = g->A[i];
        while (atual) {
            printf(" >>%.2f>>%3i", atual->peso, atual->vertice);
            atual = atual->prox;
        }
        printf("\n");
    }
}

bool libera_grafo(Grafo* g) {
    if (g == NULL) return false;

    ElemLista *atual, *apagar;

    for (int i = 0; i < g->numVertices; i++) {
        atual = g->A[i];
        while (atual) {
            apagar = atual;
            atual = atual->prox;
            free(apagar);
        }
    }

    free(g->A);

    g->numArestas = 0;
    g->numVertices = 0;
    g->A = NULL;
    return true;
}

bool aresta_existe(Grafo* g, int origem, int destino) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices) return false;
    ElemLista* atual = g->A[origem];
    while (atual && atual->vertice < destino) {
        atual = atual->prox;
    }
    if (atual && atual->vertice == destino) return true;
    return false;
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
    if (!g || nodo < 0 || nodo >= g->numVertices || !(g->A[nodo])) return false;
    return false;
}

int retorna_grau(Grafo* g, int nodo) {
    if (!g || nodo < 0 || nodo >= g->numVertices) return -1;
    int grau = 0;
    for (int i = 0; i < g->numVertices; i++) {
        ElemLista* atual = g->A[i];
        while (atual) {
            if (i == nodo) grau++;
            if (atual->vertice == nodo) grau++;
            atual = atual->prox;
        }
    }

    return grau;
}

/*
BRANCO [0] : vértice não visitado
CINZA [1] : vértice sendo processado (explorando sua lista de adjacências)
PRETO [2] : vértice já processado
*/

void visitaDFSCores(Grafo* g, int atual, int* tempo, int* cor, int* tDescoberta, int* tTermino, int* anterior) {
    (*tempo)++;
    cor[atual] = 1;
    tDescoberta[atual] = *tempo;
    int w;
    ElemLista* end = g->A[atual];
    while (end) {
        w = end->vertice;
        if (cor[w] == 0) {
            anterior[w] = atual;
            visitaDFSCores(g, w, tempo, cor, tDescoberta, tTermino, anterior);
        } else if (cor[w] == 1) printf("Este grafo não é acíclico\n");
        end= end->prox;
    }
    cor[atual] = 2;
    (*tempo)++;
    tTermino[atual] = *tempo;
}

void DFSCores(Grafo* g) {
    if (!g || g->numVertices < 1) return;
    int* cor = (int*) malloc(sizeof(int)*g->numVertices);
    int* tDescoberta = (int*) malloc(sizeof(int)*g->numVertices);
    int* tTermino = (int*) malloc(sizeof(int)*g->numVertices);
    int* anterior = (int*) malloc(sizeof(int)*g->numVertices);
    int tempo = 0;
    for (int x = 0; x < g->numVertices; x++) {
        cor[x] = 0;
        tDescoberta[x] = -1;
        tTermino[x] = -1;
        anterior[x] = -1;
    }

    for (int x = 0; x < g->numVertices; x++) {
        if (cor[x] == 0) {
            visitaDFSCores(g, x, &tempo, cor, tDescoberta, tTermino, anterior);
        }
    }
    printf("Resumo da Busca em Profundidade:\n");
    printf("No\tanterior\tDescoberta\tTermino\tCor\n");
    for (int x = 0; x < g->numVertices; x++) {
        printf("%2i\t%8i\t%10i\t%7i\t%3i\n", x, anterior[x], tDescoberta[x], tTermino[x], cor[x]);
    }

    printf("\n");
    free(cor);
    free(tDescoberta);
    free(tTermino);
    free(anterior);
}

int main() {
    Grafo *g = inicializa_grafo(5);
    inserir_aresta(g, 1, 2, 5);
    inserir_aresta(g, 3, 0, 2);
    inserir_aresta(g, 0, 2, 6);
    exibe_grafo(g);
    printf("\n");
    remove_aresta(g, 1, 2);
    exibe_grafo(g);
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

    return 1;
}