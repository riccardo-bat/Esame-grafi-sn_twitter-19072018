#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

void print(node obj){
    if(obj.tipo == 'U') printf("L'utente %s", obj.cont);
    else if(obj.tipo == 'T') printf("Il tweet %s", obj.cont);
    else printf("FORMATO NON VALIDO");
}

void copy(node* dest, node src){
    *dest = src;
}

int compare(node a, node b){
    return strcmp(a.cont, b.cont);
}

void printRelation(node src, node dest){
    if(src.tipo == 'U' && dest.tipo == 'U') printf(" FOLLOW ");
    else if(src.tipo == 'T' && dest.tipo == 'U') printf(" OWNER ");
    else if(src.tipo == 'U' && dest.tipo == 'T') printf(" LIKE ");
    else printf(" NON RECOGNIZED ");
}