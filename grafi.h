// grafi.h

#ifndef GRAFI_H
#define GRAFI_H

typedef struct adj_node {
    int node;
    float weight;
    struct adj_node* next;
} adj_node;

typedef adj_node* adj_list;

typedef struct {
    int dim;
    adj_list* nodes; // array di liste
} graph;

// Prototipi
graph new_graph(int n);
void add_arc(graph* G, int u, int v, float w);
void add_edge(graph* G, int u, int v, float w);
int get_dim(graph g);
adj_list get_adjlist(graph g, int u);
int get_adjnode(adj_list l);
adj_list get_nextadj(adj_list l);

#endif
