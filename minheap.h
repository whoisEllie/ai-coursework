#ifndef _MINHEAP_H
#define _MINHEAP_H

typedef struct
{
	int x, y, fScore, gScore, dScore;
	char* szudzik;
} keyType;

typedef struct _minheap* minheap;

minheap minheapCreate();
minheap minheapHeapify(const keyType* array, int n);
void minheapDestroy(minheap);

keyType minheapFindMin(minheap);
void minheapInsert(minheap, keyType);
void minheapDeleteMin(minheap);

int minheapIsEmpty(minheap);
int minheapSize(minheap);
void minheapClear(minheap);

#endif
