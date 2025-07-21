#include <stdio.h>
#include <stdlib.h>
#include "coda.h"

// Funzione per creare un nuovo elemento della lista
static elem* new_elem(tipo_inf inf) {
    elem* p = (elem*)malloc(sizeof(elem));
    if (p == NULL) {
        fprintf(stderr, "Errore allocazione memoria\n");
        exit(1);
    }
    p->inf = inf;
    p->pun = NULL;
    return p;
}

// Inserisce un elemento in coda (enqueue)
coda enqueue(coda c, tipo_inf i) {
    elem* e = new_elem(i);

    if (c.tail != NULL)
        c.tail->pun = e;

    c.tail = e;

    if (c.head == NULL)
        c.head = c.tail;

    return c;
}

// Rimuove il primo elemento (dequeue)
tipo_inf dequeue(coda* c) {
    if (c->head == NULL) {
        fprintf(stderr, "Errore: dequeue su coda vuota\n");
        exit(1);
    }

    tipo_inf ris = c->head->inf;
    elem* app = c->head;
    c->head = c->head->pun;

    free(app);

    if (c->head == NULL)
        c->tail = NULL;

    return ris;
}

// Restituisce il primo elemento senza rimuoverlo
tipo_inf first(coda c) {
    if (c.head == NULL) {
        fprintf(stderr, "Errore: first su coda vuota\n");
        exit(1);
    }
    return c.head->inf;
}

// Controlla se la coda è vuota
int isEmpty(coda c) {
    return (c.head == NULL);
}

// Crea una nuova coda vuota
coda newQueue(void) {
    coda c = {NULL, NULL};
    return c;
}
