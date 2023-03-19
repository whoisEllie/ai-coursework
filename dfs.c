#include "./Hashmap/hashmap.h"
#include "stack.h"
#include "fileloader.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define DEBUG false 
#define OUTPUT true
#define FORCESINGLESEARCHPATTERN false 

// Structs

struct node *workingStack;
struct node *reverseStack;

// Hashmap helpers

char* longToStr(long input)
{
    char* str = malloc((int)((ceil(log10(input))+1)*sizeof(char)));
    sprintf(str, "%ld", input);
    return str;
}

int coordsCompare(const void *a, const void *b, void *udata)
{
    const coords *ca = a;
    const coords *cb = b;
    return strcmp(ca->szudzik, cb->szudzik);
}

bool coordsIter(const void *item, void *data)
{
    const coords *coords = item;
    printf("Visited node {%d,%d} with encoding %s\n", coords->x, coords->y, coords->szudzik);
    return true;
}

uint64_t coordsHash(const void *item, uint64_t seed0, uint64_t seed1)
{
    const coords *coords = item;
    return hashmap_sip(coords->szudzik, strlen(coords->szudzik), seed0, seed1);
}

// Helper functions for coordinates

char* szudzikEncode(int a, int b)
{
    return longToStr(a >= b? a * a + a + b : a + b * b);
}

// DFS implementation

void pushNextNeighborToStack(struct mazeArray *inArray, struct hashmap *visitedNodes, coords inCoords)
{
    if (inArray->columns >= inArray->rows || FORCESINGLESEARCHPATTERN) { // We check this in order to be more efficient at solving mazes of different sizes
        if (inCoords.y-1 >= 0) {
            char nodeLeft = inArray->arr[inCoords.x][inCoords.y-1];
            if (nodeLeft == '-') {
                coords neighborCoords = {inCoords.x, inCoords.y-1, szudzikEncode(inCoords.x, inCoords.y-1)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
        if (inCoords.y+1 < inArray->columns) {
            char nodeRight = inArray->arr[inCoords.x][inCoords.y+1];
            if (nodeRight == '-') {
                coords neighborCoords = {inCoords.x, inCoords.y+1, szudzikEncode(inCoords.x, inCoords.y+1)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
        if (inCoords.x-1 >= 0) {
            char nodeAbove = inArray->arr[inCoords.x-1][inCoords.y];	
            if (nodeAbove == '-') {
                coords neighborCoords = {inCoords.x-1, inCoords.y, szudzikEncode(inCoords.x-1, inCoords.y)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
        if (inCoords.x+1 < inArray->rows) {
            char nodeBelow = inArray->arr[inCoords.x+1][inCoords.y];
            if (nodeBelow == '-') {
                coords neighborCoords = {inCoords.x+1, inCoords.y, szudzikEncode(inCoords.x+1, inCoords.y)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
    }
    else {
        if (inCoords.x-1 >= 0) {
            char nodeAbove = inArray->arr[inCoords.x-1][inCoords.y];	
            if (nodeAbove == '-') {
                coords neighborCoords = {inCoords.x-1, inCoords.y, szudzikEncode(inCoords.x-1, inCoords.y)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
        if (inCoords.x+1 < inArray->rows) {
            char nodeBelow = inArray->arr[inCoords.x+1][inCoords.y];
            if (nodeBelow == '-') {
                coords neighborCoords = {inCoords.x+1, inCoords.y, szudzikEncode(inCoords.x+1, inCoords.y)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
        if (inCoords.y-1 >= 0) {
            char nodeLeft = inArray->arr[inCoords.x][inCoords.y-1];
            if (nodeLeft == '-') {
                coords neighborCoords = {inCoords.x, inCoords.y-1, szudzikEncode(inCoords.x, inCoords.y-1)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
        if (inCoords.y+1 < inArray->columns) {
            char nodeRight = inArray->arr[inCoords.x][inCoords.y+1];
            if (nodeRight == '-') {
                coords neighborCoords = {inCoords.x, inCoords.y+1, szudzikEncode(inCoords.x, inCoords.y+1)};
                if (!hashmap_get(visitedNodes, &neighborCoords)) {
                    push(&workingStack, neighborCoords);
                    return;
                }
            }
        }
    }
  
    if (DEBUG) {
        printf("Found no new neighbors for {%d,%d}\n", inCoords.x, inCoords.y);
    }
    // If the node has no unvisited neighbors, pop it from the stack
    pop(&workingStack);
}

bool depthFirstSearch(struct mazeArray *inArray, struct hashmap *visitedNodes, coords startingCoords, coords endingCoords)
{
	  push(&workingStack, startingCoords);

	  while (!isEmpty(&workingStack)) {
		    coords currentNode = peek(&workingStack);
        hashmap_set(visitedNodes, &currentNode);

        if (!strcmp(currentNode.szudzik, endingCoords.szudzik)) 
        {
            if (DEBUG) {
                printf("\n\nFound a path!!!\n\n\n");
            }
            return true;
        }

        pushNextNeighborToStack(inArray, visitedNodes, currentNode);
	  }

    return false;
}

int main(int argc, char *argv[])
{
    if (argc == 2) {

        if (fopen(argv[1], "r") == NULL) {
            printf("Please enter a valid path to a maze.txt file and try again.\n");
            return 1;
        }

        // Creating the hashmap
        struct hashmap *visitedNodes = hashmap_new(sizeof(coords), 0, 0, 0, coordsHash, coordsCompare, NULL, NULL);

        // Creating the maze
        struct mazeArray mazeGrid = loadFile(argv[1]);

        // Finding the starting and ending nodes
        int startingNodeIndex = findPathIndexInArray(mazeGrid.arr[0], mazeGrid.columns);
        int endingNodeIndex = findPathIndexInArray(mazeGrid.arr[mazeGrid.rows - 1], mazeGrid.columns);

        if (DEBUG) {
            printf("Starting node coords: {%d,%d}\n", 0, startingNodeIndex);
            printf("Ending node coords: {%d,%d}\n", mazeGrid.rows - 1, endingNodeIndex);
        }

        coords startingCoords = {0, startingNodeIndex, szudzikEncode(0, startingNodeIndex)};
        coords endingCoords = {mazeGrid.rows - 1, endingNodeIndex, szudzikEncode(mazeGrid.rows-1, endingNodeIndex)};
      
        clock_t begin = clock();

        // Performing the depth-first search
        if (depthFirstSearch(&mazeGrid, visitedNodes, startingCoords, endingCoords))
        {
            clock_t end = clock();

            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

            // If we have a successful result: 

            char *outPath = argv[1];
            int len = strlen(outPath);
            outPath[len-4] = '\0';
            strncat(outPath, "-solution-dfs.txt", 17);

            // Calculate the amount of nodes in the path and output the final solution
            
            int finalPathNodes = 0;
            FILE* solutionFile;
            solutionFile = fopen(outPath, "w");

            fprintf(solutionFile, "The solution to the maze takes the following path:\n");

            while (!isEmpty(&workingStack)) {
                coords nextCoords = pop(&workingStack); 
                push(&reverseStack, nextCoords);
                mazeGrid.arr[nextCoords.x][nextCoords.y] = 'o';
                finalPathNodes++;
            }

            bool writing = false;

            while (!isEmpty(&reverseStack)) {
                if (writing) {
                    fprintf(solutionFile, ", ");
                }
                coords nextCoords = pop(&reverseStack); 
                fprintf(solutionFile, "{%d, %d}", nextCoords.x, nextCoords.y);
                writing = true;
            }

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
                strncat(outPath, "-dfs.ppm", 8);
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
