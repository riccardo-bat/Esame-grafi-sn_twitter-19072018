#ifndef CODA_H
#define CODA_H

// Definisci tipo_inf come vuoi (es. int, float, struct...)
typedef int tipo_inf;

typedef struct elem {
    tipo_inf inf;
    struct elem* pun;
} elem;

typedef elem* lista;

typedef struct {
    lista head;
    lista tail;
} coda;

// Prototipi delle funzioni
coda enqueue(coda c, tipo_inf i);
tipo_inf dequeue(coda* c);      // usa puntatore per modificare la coda
tipo_inf first(coda c);
int isEmpty(coda c);
coda newQueue(void);

#endif
