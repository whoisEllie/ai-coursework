#ifndef _MINHEAP_H
#define _MINHEAP_H

typedef struct Node
{
	int x, y, fScore, gScore, hScore;
	struct Node *parent;
	char* szudzik;
} Node;

typedef struct _minheap* minheap;

minheap minheapCreate();
minheap minheapHeapify(const Node** array, int n);
void minheapDestroy(minheap);

Node* minheapFindMin(minheap);
void minheapInsert(minheap, Node*);
void minheapDeleteMin(minheap);

int minheapIsEmpty(minheap);
int minheapSize(minheap);
void minheapClear(minheap);

#endif
