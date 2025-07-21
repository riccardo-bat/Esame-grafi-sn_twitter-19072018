#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include "grafi.h"

//prototipi 
graph load_graph_from_file(char* filename);
void stampa(graph grafo_twitter);


int main(){
    //punto 1
    graph grafo_twitter = load_graph_from_file("graph.txt");
    stampa(grafo_twitter);


    printf("\n\n");
    return 0;
}

/**
 * @brief Funzione che acquisisce un grafo da un file testuale, restituendo l'oggetto di tipo graph
 * 
 * @param filename 
 * @return graph 
 */
graph load_graph_from_file(char* filename){
    char* mode = "r";

    FILE* fp_grafo = fopen(filename, mode);
    if(fp_grafo == NULL){printf("\nERRORE NELL'APERTURA DEL FILE"); exit(EXIT_FAILURE);}

    //lettura del file
    char buffer[256];
    int totNodi = 0;

    //la prima riga contiene il numero totale degli archi
    if(fgets(buffer, sizeof buffer, fp_grafo) == NULL){printf("\nERRORE IN LETTURA"); fclose(fp_grafo); exit(EXIT_FAILURE);}
    buffer[strcspn(buffer, "\r\n")] = '\0';
    if(sscanf(buffer, " %d", &totNodi) != 1){printf("\nRIGA NON VALIDA"); fclose(fp_grafo); exit(EXIT_FAILURE);}

    //creo l'oggetto grafo dopo aver ottenuto il numero totale di nodi presenti 
    graph grafo_twitter = new_graph(totNodi);

    //dalla seconda riga è presente una riga x ogni arco presente
    while(fgets(buffer, sizeof buffer, fp_grafo) != NULL){
        buffer[strcspn(buffer, "\r\n")] = '\0';
        //printf("\nRIGA LETTA: %s", buffer);

        //se la riga è valida
        if(buffer[0] != '\0'){
            //leggo l'arco sorgente --> destinazione
            int sorgente = 0, destinazione = 0;
            if(sscanf(buffer, " %d %d", &sorgente, &destinazione) != 2){printf("\nRIGA NON VALIDA"); fclose(fp_grafo); exit(EXIT_FAILURE);}

            //se la riga è valida, inserisco l'arco all'interno del grafo
            //printf("\nArco letto: %d -> %d", sorgente, destinazione);
            add_arc(&grafo_twitter, sorgente, destinazione, 1.0);
        }

        //else --> non considero righe vuote o non valida

    }

    //chisuura del file
    if(fclose(fp_grafo) == EOF){printf("\nERRORE NELLA CHIUSURA DEL FILE"); exit(EXIT_FAILURE);}

    //ripristino dei valori e ritorno
    return grafo_twitter;
}

/**
 * @brief Procedura di stampa degli archi del grafi
 * 
 * @param grafo_twitter 
 */
void stampa(graph grafo_twitter){
    for(int node=0; node<get_dim(grafo_twitter); node++){
        printf("\nVicini del nodo %d:", node+1);

        adj_list cursor = get_adjlist(grafo_twitter, node+1);
        //scorro la lista dei vicini del node node+1
        while(cursor != NULL){
            printf("\n\t- %d", get_adjnode(cursor));
            cursor = get_nextadj(cursor);
        }

    }

}





