#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "minheap.h"

int heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int main(int argc, char* argv[])
{
    int i;
    srand(time(NULL));

    minheap heap = minheapCreate();

    for (i = 0; i < 10000; i++) {
        minheapInsert(heap, rand() % 100000);
    }

    while (!minheapIsEmpty(heap)) {
        printf("%8d", minheapFindMin(heap));
        minheapDeleteMin(heap);
    }

    minheapDestroy(heap);

    return 0;
}
