// grafi.c

#include <stdio.h>
#include <stdlib.h>
#include "grafi.h"

// Crea un nuovo grafo con n nodi
graph new_graph(int n) {
    graph G;
    G.dim = n;
    G.nodes = (adj_list*)malloc(n * sizeof(adj_list));
    if (G.nodes == NULL) {
        fprintf(stderr, "Errore: allocazione memoria fallita.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        G.nodes[i] = NULL;
    }

    return G;
}

// Aggiunge un arco orientato (u -> v) con peso w
void add_arc(graph* G, int u, int v, float w) {
    adj_node* t = (adj_node*)malloc(sizeof(adj_node));
    if (t == NULL) {
        fprintf(stderr, "Errore: allocazione nodo adiacente fallita.\n");
        exit(1);
    }

    t->node = v - 1;  // converti da 1-based a 0-based
    t->weight = w;
    t->next = G->nodes[u - 1];
    G->nodes[u - 1] = t;
}

// Aggiunge un arco non orientato tra u e v
void add_edge(graph* G, int u, int v, float w) {
    add_arc(G, u, v, w);
    add_arc(G, v, u, w);
}

// Restituisce il numero di nodi
int get_dim(graph g) {
    return g.dim;
}

// Restituisce la lista di adiacenza del nodo u
adj_list get_adjlist(graph g, int u) {
    return g.nodes[u - 1];  // converti da 1-based a 0-based
}

// Restituisce l'indice del nodo adiacente
int get_adjnode(adj_list l) {
    return l->node + 1; // converti da 0-based a 1-based
}

// Restituisce il prossimo nodo adiacente
adj_list get_nextadj(adj_list l) {
    return l->next;
}
