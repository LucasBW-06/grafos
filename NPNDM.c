// Grafo não ponderado não direcional por matriz de adjacencias

#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

typedef struct grafo {
    int numVertices;
    int numArestas;
    bool** matriz;
} Grafo;

typedef struct auxNo {
    int valor;
    struct auxNo* prox;
    
} No;

typedef struct {
    No* inicio;
    No* fim;
} Fila;

Grafo* inicializa_grafo(int vertices) {
    if (vertices < 1) return NULL;
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->numArestas = 0;
    g->numVertices = vertices;

    g->matriz = (bool**) malloc(vertices * sizeof(bool*));

    for (int i = 0; i < vertices; i++) {
        g->matriz[i] = (bool*)malloc(vertices * sizeof(bool));
        for (int j = 0; j < vertices; j++) {
            g->matriz[i][j] = false;
        }
    }

    return g;
}

bool inserir_aresta(Grafo* g, int origem, int destino) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices || origem == destino) return false;
    if (g->matriz[origem][destino] == false) {
        g->matriz[origem][destino] = true;
        g->matriz[destino][origem] = true;
        g->numArestas++;
    }
    return true;
}

bool remove_aresta(Grafo* g, int origem, int destino) {
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices || g->matriz[origem][destino] == false) return false;
    g->matriz[origem][destino] = false;
    g->matriz[destino][origem] = false;
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
        for (int j = 0; j < g->numVertices; j++) printf("\t%5i", g->matriz[i][j]);
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
    if (!g || origem < 0 || destino < 0 || origem >= g->numVertices || destino >= g->numVertices || g->matriz[origem][destino] == false) return false;
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
        if (g->matriz[nodo][i]) return true;
    }
    return false;
}

int retorna_grau(Grafo* g, int nodo) {
    if (!g || nodo < 0 || nodo >= g->numVertices) return -1;
    int grau = 0;
    for (int i = 0; i < g->numVertices; i++) {
        if (g->matriz[nodo][i]) grau++; 
    }

    return grau;
}

void visitaProfundidade(Grafo* g, int atual, bool* visitado, int anterior) {
    printf("Visitando vertice: %3i (anterior: %3i)\n", atual, anterior);
    visitado[atual] = true;
    for (int x = 0; x < g->numVertices; x++) {
        if (g->matriz[atual][x] && !visitado[x]) {
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

void inicializaFila(Fila* f) {
    f->inicio = NULL;
    f->fim = NULL;
}

bool filaVazia(Fila* f) {
    if (!f->inicio) return true;
    return false;
}

void insereFila(Fila* f, int valor) {
    No* novo = (No*) malloc(sizeof(No));
    novo->prox = NULL;
    novo->valor = valor;
    if (f->inicio == NULL) {
        f->inicio = novo;
        f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }
}

int excluiFila(Fila* f) {
    if (!f->inicio) return -1;
    No* atual = f->inicio;
    int valor = atual->valor;
    f->inicio = atual->prox;
    if (!f->inicio) f->fim = NULL;
    free(atual);
    return valor;
}

void buscaEmLarguraAux(Grafo* g, int inicial, bool* visitado) {
    if (!g || inicial < 0 || inicial >= g->numVertices) return;
    int atual;
    Fila f;
    inicializaFila(&f);
    insereFila(&f, inicial);
    visitado[inicial] = true;
    while(!filaVazia(&f)) {
        atual = excluiFila(&f);
        for (int x = 0; x < g->numVertices; x++) {
            if (g->matriz[atual][x] && !visitado[x]) {
                insereFila(&f, x);
                visitado[x] = true;
            }
        }
    }
}

void buscaEmLargura(Grafo* g, int inicial) {
    if (!g || g->numVertices < 1) return;
    bool* visitado = (bool*) malloc(sizeof(bool) * g->numVertices);
    for (int i = 0; i < g->numVertices; i++) {
        visitado[i] = false;
    }
    for (int i = 0; i < g->numVertices; i++) {
        if (!visitado[i]) {
            buscaEmLarguraAux(g, i, visitado);
        }
    }

    free(visitado);
}

int main() {
    Grafo *g = inicializa_grafo(5);
    inserir_aresta(g, 1, 2);
    inserir_aresta(g, 3, 0);
    inserir_aresta(g, 0, 2);
    exibe_grafo(g);
    printf("\n");
    buscaEmProfundidade(g);
    printf("\n");
    buscaEmLargura(g, 0);
    printf("\n");
    remove_aresta(g, 1, 2);
    exibe_grafo(g);
    printf("\n");
    buscaEmProfundidade(g);
    printf("\n");
    buscaEmLargura(g, 0);
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
    printf("\n");
    buscaEmLargura(g, 0);

    return 1;
}