#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "minheap.h"
#include "hashmap.h"
#include "stack.h"
#include "fileloader.h"

#define OUTPUT true
#define DEBUG false

struct node *outStack;

// Hashmap helpers

char* longToStr(long input)
{
    char* str = malloc((int)((ceil(log10(input))+1)*sizeof(char)));
    sprintf(str, "%ld", input);
    return str;
}

int keyTypeCompare(const void *a, const void *b, void *udata)
{
    const keyType *ka = a;
    const keyType *kb = b;
    return strcmp(ka->szudzik, kb->szudzik);
}

bool keyTypeIter(const void *item, void *data)
{
    const keyType *keyData = item;
    printf("Visited node {%d,%d} with encoding %s\n", keyData->x, keyData->y, keyData->szudzik);
    return true;
}

uint64_t keyTypeHash(const void *item, uint64_t seed0, uint64_t seed1)
{
    const keyType *keyData= item;
    return hashmap_sip(keyData->szudzik, strlen(keyData->szudzik), seed0, seed1);
}

int heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// Helper functions for coordinates

char* szudzikEncode(int a, int b)
{
    return longToStr(a >= b? a * a + a + b : a + b * b);
}

bool astar()
{
    // Enqueue starting node
    //
    // While priority queue is not empty
    //      If the current node is our end point, break and return
    //
    //      Dequeue the current node
    //      Enqueue any surrounding nodes
    //      Update the evaluation and distance functions

    return false;
}

/*int main(int argc, char* argv[])
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
}*/


int main(int argc, char *argv[])
{
    if (argc == 2) {

        if (fopen(argv[1], "r") == NULL) {
            printf("Please enter a valid path to a maze.txt file and try again.\n");
            return 1;
        }

        // Creating the hashmap
        struct hashmap *visitedNodes = hashmap_new(sizeof(coords), 0, 0, 0, keyTypeHash, keyTypeCompare, NULL, NULL);

        // Creating the maze
        struct mazeArray mazeGrid = loadFile(argv[1]);

        // Finding the starting and ending nodes
        int startingNodeIndex = findPathIndexInArray(mazeGrid.arr[0], mazeGrid.columns);
        int endingNodeIndex = findPathIndexInArray(mazeGrid.arr[mazeGrid.rows - 1], mazeGrid.columns);

        if (DEBUG) {
            printf("Starting node coords: {%d,%d}\n", 0, startingNodeIndex);
            printf("Ending node coords: {%d,%d}\n", mazeGrid.rows - 1, endingNodeIndex);
        }

        keyType startingCoords = {0, startingNodeIndex, 0, 0, 0, szudzikEncode(0, startingNodeIndex)};
        keyType endingCoords = {mazeGrid.rows - 1, endingNodeIndex, 0, 0, 0, szudzikEncode(mazeGrid.rows-1, endingNodeIndex)};
      
        clock_t begin = clock();

        // Performing the A* search
        if (astar())
        {
            clock_t end = clock();

            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

            // If we have a successful result: 

            char *outPath = argv[1];
            int len = strlen(outPath);
            outPath[len-4] = '\0';
            strncat(outPath, "-solution.txt", 13);

            // Calculate the amount of nodes in the path and output the final solution
            
            int finalPathNodes = 0;
            FILE* solutionFile;
            solutionFile = fopen(outPath, "w");

            fprintf(solutionFile, "The solution to the maze takes the following path:\n");

            // Trace up the output path and push each node into the outStack

            while (!isEmpty(&outStack)) {
                coords nextCoords = pop(&outStack); 
                fprintf(solutionFile, "{%d, %d}, ", nextCoords.x, nextCoords.y);
            }

            fprintf(solutionFile, "\n");
            fclose(solutionFile);

            printf("//////////////////////////////////\n");
            printf("/  Explored %zu nodes\n", hashmap_count(visitedNodes));
            printf("/  The final path contains %d nodes\n", finalPathNodes);
            printf("/  The program took %fs to compute the final path\n", time_spent);
            printf("/  The computed solution was stored in ./%s\n", outPath);

            // Freeing hashmap
            hashmap_free(visitedNodes);

            // Generating the output image
            if (OUTPUT) {

                FILE* outFile;

                outPath[len-4] = '\0';
                strncat(outPath, ".ppm", 4);
                printf("/  A visualization of the computed solution was stored in ./%s\n", outPath);
                outFile = fopen(outPath, "w");

                fprintf(outFile, "P3 %d %d 255\n", mazeGrid.columns, mazeGrid.rows);

                int j, i;

                for (i = 0; i < mazeGrid.rows; i++) {
                    for (j = 0; j < mazeGrid.columns; j++) {
                        if (mazeGrid.arr[i][j] == '#') {
                            fprintf(outFile, "0 0 0\n");
                        }
                        else if (mazeGrid.arr[i][j] == '-') {
                            fprintf(outFile, "255 255 255\n");
                        }
                        else if (mazeGrid.arr[i][j] == 'o') {
                            fprintf(outFile, "255 0 0\n");
                        }
                    }
                }  
            }

            printf("//////////////////////////////////\n");

            return 0;
        }

        printf("No solutions found, check maze and try again\n");
        return 1;
    }
    else if (argc == 1) {
        printf("Please include the path of the maze you wish to scan as an argument\n");
        return 1;
    }
    else {
        printf("Please include just one argument - the path to the maze you wish to scan\n");
        return 1;
    }
}
