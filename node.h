#ifndef NODE_H
#define NODE_H

typedef struct {
	char cont[81];
	char tipo;
}node;

void print(node);
void copy(node*, node);
int compare(node, node);
void printRelation(node, node);

#endif
