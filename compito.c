#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include "grafi.h"
#include "node.h"
#include "coda.h"

//prototipi 
long int int_input(char* msg, int minimum_value, int max_value);
graph load_graph_from_file(char* filename);
void stampa(graph grafo_twitter, node* arrayNodes);
node* load_nodes_from_file(int numeroNodi, char* filename);
void follow(graph, node*, int);
int* totalLike(graph grafo_twitter, node* arrayNodes);


int main(){
    //punto 1
    graph grafo_twitter = load_graph_from_file("graph.txt");
    //stampa(grafo_twitter);

    //punto 2
    printf("\n---------------");
    node* arrayNodes = load_nodes_from_file(get_dim(grafo_twitter), "node.txt");
    stampa(grafo_twitter, arrayNodes);

    //punto 3b
    printf("\n---------------");
    follow(grafo_twitter, arrayNodes, (int) int_input("\nInserire l'id di un nodo di cui effettuare la ricerca: ", 1, get_dim(grafo_twitter)) );


    //punto 3a
    printf("\n---------------");
    int* likes = totalLike(grafo_twitter, arrayNodes);

    //cerco il numero di like massimi 
    int max = likes[0];
    for(int i=1; i<get_dim(grafo_twitter); i++){
        if(likes[i] > max)
            max = likes[i];
    }

    printf("\nI MIP sono: ");
    //stampo gli utenti che hanno il #likes massimo
    for(int i=0; i<get_dim(grafo_twitter); i++){
        if(likes[i] == max){
            printf("\n\t");
            print(arrayNodes[i]);
            printf(", dal totale di %d likes", likes[i]);
        }
    }


    //libero la memoria
    free(arrayNodes);

    printf("\n\n");
    return 0;
}

long int int_input(char* msg, int minimum_value, int max_value){
    long int input;
    char buffer[100];

    printf("%s", msg);
    while(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\nErrore nella lettura dell'input. Riprova: ");
    }

    char *endptr;
    input = strtol(buffer, &endptr, 10); //per altri formati, strtof, strtod, ...
    if (endptr == buffer || *endptr != '\n' || input < minimum_value || input > max_value) {
        printf("\nInput non valido\n.");
        return int_input(msg, minimum_value, max_value);
    }

    return input;
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
/*void stampa(graph grafo_twitter){
    for(int node=0; node<get_dim(grafo_twitter); node++){
        printf("\nVicini del nodo %d:", node+1);

        adj_list cursor = get_adjlist(grafo_twitter, node+1);
        //scorro la lista dei vicini del node node+1
        while(cursor != NULL){
            printf("\n\t- %d", get_adjnode(cursor));
            cursor = get_nextadj(cursor);
        }

    }

}*/

/**
 * @brief Funzione di stampa in formato verbose del grafo
 * 
 */

void stampa(graph grafo_twitter, node* arrayNodes){
    //stampa in formato verbose
    printf("\n\nSTAMPA IN FORMATO VERBOSE\n");

    //scorro l'array del grafo
    for(int src=0; src<get_dim(grafo_twitter); src++){
        printf("\n");

        //scorro i vicini del nodo src+1
        adj_list cursor = get_adjlist(grafo_twitter, src+1); 
        while(cursor != NULL){
            printf("\n%s", arrayNodes[src].cont);
            printRelation(arrayNodes[src], arrayNodes[cursor->node]);
            printf("%s", arrayNodes[cursor->node].cont);

            cursor = get_nextadj(cursor);
        }

    }
}


/**
 * @brief Caricamento e restituzione di un array dinamico che contiene i nodi del grafo
 * 
 * @param numeroNodi 
 * @param filename 
 * @return node* 
 */
node* load_nodes_from_file(int numeroNodi, char* filename){
    //creo l'array dinamicamente
    node* arrayNodes = malloc(numeroNodi * sizeof(node));
    if(arrayNodes == NULL){printf("\nMALLOC FAILED"); exit(EXIT_FAILURE);}

    char* mode = "r";

    FILE* fp_nodes = fopen(filename, mode);
    if(fp_nodes == NULL){printf("\nERRORE NELL'APERTURA DEL FILE"); exit(EXIT_FAILURE);} 


    //leggo il file
    char buffer[256];

    //di ogni nodo vengono lette due righe
    int riga_per_nodo = 0;
    node tmp_node; 
    int index_last_elem = -1;

    while(fgets(buffer, sizeof(buffer), fp_nodes) != NULL){
        buffer[strcspn(buffer, "\r\n")] = '\0';
        printf("\nRIGA LETTA: %s", buffer);

        //se ho letto una riga valida
        if(buffer[0] != '\0'){
            riga_per_nodo++; 

            //in base alla riga che ho letto, verifico il tipo di dato da leggere
            switch(riga_per_nodo){
                case 1:{
                    //la prima riga contiene il contenuto
                    //non essendoci altri valori nella riga, posso direttamente copiare il contenuto
                    strcpy(tmp_node.cont, buffer);
                    break;
                }

                case 2:{
                    //la seconda riga contiene il tipo di nodo da salvare
                    if(sscanf(buffer, " %c", &tmp_node.tipo) != 1){printf("\nRIGA NON VALIDA"); fclose(fp_nodes); exit(EXIT_FAILURE);}
                    if(tmp_node.tipo != 'U' && tmp_node.tipo != 'T'){printf("\nRIGA NON VALIDA"); fclose(fp_nodes); exit(EXIT_FAILURE);}
                    break;
                }

            }

            //se ho letto un nodo intero, lo salvo e ripristino i valori
            if(riga_per_nodo == 2){
                printf("\nNODO DA AGGIUNGERE: ");
                print(tmp_node);
                //salvo il nodo nell'array
                index_last_elem++;
                arrayNodes[index_last_elem] = tmp_node;

                //ripristino i valori
                riga_per_nodo = 0;
            }


        } 

        //else --> non considero le righe non valide

    }


    if(fclose(fp_nodes) == EOF){printf("\nERRORE NELLA CHIUSURA DEL FILE"); exit(EXIT_FAILURE);}
    return arrayNodes;
}

/**
 * @brief Procedura che stampa i nodi che il nodo id_node segue indirettamente o direttamente
 * 
 * @param grafo_twitter 
 * @param arrayNodes 
 * @param id_nodo 
 */
void follow(graph grafo_twitter, node* arrayNodes, int id_nodo){
    //utilizzo una visita BFS

    coda queue = newQueue();
    bool* visited = malloc(get_dim(grafo_twitter) * sizeof(bool));
    //controllo malloc

    //inserisco la sorgente della visita
    queue = enqueue(queue, id_nodo); //1-based
    visited[id_nodo-1] = true;

    printf("L'utente %s segue: ", arrayNodes[id_nodo-1].cont);
    while(isEmpty(queue) != 1){
        int u = dequeue(&queue);

        //stampo l'utente che id_nodo segue direttamente o indirettamente, tranne id_node stesso
        if(u != id_nodo) printf("\n\t%s", arrayNodes[u-1].cont);

        //verifico tutti gli utenti seguiti da u
        adj_list cursor = get_adjlist(grafo_twitter, u);
        while(cursor != NULL){
            //se u segue un nodo utente (diverso da id_node) lo inserisco nella coda <=> non è già stato entrato in coda
            if(arrayNodes[cursor->node].tipo == 'U' && !visited[cursor->node]){
                queue = enqueue(queue, cursor->node+1);
                visited[cursor->node] = true;
            }

            cursor = get_nextadj(cursor);
        }

    }

    free(visited);
}

/**
 * @brief id_twitter 1-based. Return 1-based
 * 
 * @param grafo_twitter 
 * @param id_twitter 
 * @return int 
 */
int find_owner_tweet(graph grafo_twitter, node* arrayNodes, int id_twitter){
    //considero solo i vicini del twitter
    adj_list cursor = get_adjlist(grafo_twitter, id_twitter);
    //cerco la relazione T-->U
    while(cursor != NULL){
        if(arrayNodes[cursor->node].tipo == 'U')  
            return cursor->node+1;

        cursor = cursor->next;
    }

    return -1;
}

/**
 * @brief Funzione che restituisce il numero di like ai tweet ricevuti per ogni utente
 * 
 */
int* totalLike(graph grafo_twitter, node* arrayNodes){
    int* likes = malloc(get_dim(grafo_twitter) * sizeof(int));
    for(int i=0; i<get_dim(grafo_twitter); i++) likes[i] = 0;

    //scorro tutto il grafo per verifico il #totale di like ricevuti da ogni utente per i propri tweet
    for(int node=0; node<get_dim(grafo_twitter); node++){
        //considero solo i nodi utente e le relazioni U->T
        if(arrayNodes[node].tipo == 'U'){
            //cerco una relazione -->T
            adj_list cursor = get_adjlist(grafo_twitter, node+1);
            while(cursor != NULL){
                //se il vicino di node+1 è un tweet
                if(arrayNodes[cursor->node].tipo == 'T'){
                    //cerco l'owner del tweet dall'id pari a cursor->node+1
                    likes[find_owner_tweet(grafo_twitter, arrayNodes, cursor->node+1) - 1] += 1;
                }

                //else --> non mi interessano le relazioni di follow

                cursor = get_nextadj(cursor);
            }


        }

        //else --> non considero i tweet


    }

    return likes;

}


