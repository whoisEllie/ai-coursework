#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minheap.h"

struct _minheap {
    keyType* array;
    int maxSize;
    int curSize;
};

minheap minheapCreate() {
    minheap heap = (minheap)malloc(sizeof(struct _minheap));

    if (heap == NULL) {
        printf("Not enough memory!\n");
        abort();
    }

    heap->maxSize = 64;
    heap->curSize = 0;
    heap->array = (keyType*)malloc(sizeof(keyType)*(heap->maxSize+1));

    if (heap->array == NULL) {
        printf("Not enough memory!\n");
        abort();
    }

    return heap;
}

void minheapDestroy(minheap heap) {
    assert(heap);
    free(heap->array);
    free(heap);
}

static void minheapDoubleCapacity(minheap heap) {
    int newMaxSize = 2 * heap->maxSize;
    keyType* newArray = (keyType*)malloc(sizeof(keyType)*(newMaxSize+1)); 

    if (newArray == NULL) {
        printf("Not enough memory!\n"); 
        abort();
    }

    for (int i = 1; i <= heap->curSize; i++) {
        newArray[i] = heap->array[i];
    }

    free(heap->array);
    heap->array = newArray;
    heap->maxSize = newMaxSize;
}

static void minheapSwap(minheap heap, int i, int j) {
    assert(heap && i >= 1 && i <= heap->curSize && j >= 1 && j <= heap->curSize);
    keyType temp = heap->array[i];
    heap->array[i] = heap->array[j];
    heap->array[j] = temp;
}

static void minheapFixup(minheap heap, int k) {
    assert(heap && k >= 1 && k <= heap->curSize);
    
    while (k > 1 && heap->array[k] < heap->array[k/2]) {
        minheapSwap(heap, k/2, k);
        k /= 2;
    }
}

static void minheapFixdown(minheap heap, int k) {
    assert(heap);

    while (2*k <= heap->curSize) {
        int j = 2*k; 

        if (j < heap->curSize && heap->array[j+1] < heap->array[j]) {
            j++; 
        }

        if (heap->array[k] <= heap->array[j]) {
            break;
        }

        minheapSwap(heap, k, j);
        k = j;
    }
}

void minheapInsert(minheap heap, keyType key) {
    assert(heap);

    // Make sure that there is enough space to insert
    if (heap->curSize == heap->maxSize) {
        minheapDoubleCapacity(heap); 
    }

    heap->array[++heap->curSize] = key;

    minheapFixup(heap, heap->curSize);
}

int minheapFindMin(minheap heap) {
    if (minheapIsEmpty(heap)) {
        printf("Heap is empty!\n"); 
        abort();
    }

    return heap->array[1];
}

void minheapDeleteMin(minheap heap) {
    if (minheapIsEmpty(heap)) {
        printf("Heap is empty!\n"); 
        abort();
    }

    minheapSwap(heap, 1, heap->curSize);

    heap->curSize--;

    minheapFixdown(heap, 1);
}

int minheapSize(minheap heap) {
    assert(heap);
    return heap->curSize;
}

int minheapIsEmpty(minheap heap) {
    assert(heap);
    return heap->curSize <= 0;
}

void minheapClear(minheap heap) {
    assert(heap);
    heap->curSize = 0;
}

minheap minheapHeapify(const keyType *array, int n) {
    assert(array && n > 0);

    minheap heap = (minheap)malloc(sizeof(struct _minheap));

    if (heap == NULL) {
        printf("Not enough memory!\n"); 
        abort();
    }

    heap->maxSize = n;
    heap->curSize = 0;
    heap->array = (keyType*)malloc(sizeof(keyType)*(heap->maxSize+1));

    if (heap->array == NULL) {
        printf("Not enough memory!"); 
        abort();
    }

    heap->curSize = n;
    for (int k = 0; k < n; k++) {
        heap->array[k+1] = array[k]; 
    }

    for (int k = (heap->maxSize+1)/2; k > 0; k--) {
        minheapFixdown(heap, k); 
    }

    return heap;
}
