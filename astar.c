#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "minheap.h"
#include "./Hashmap/hashmap.h"
#include "stack.h"
#include "fileloader.h"

#define OUTPUT true
#define DEBUG false

struct node *outStack;

// Hashmap helpers

char* longToStr(long input)
{
    char* str = malloc((int)((ceil(log10(input))+1)*sizeof(char)));
    if (str != NULL) {
        sprintf(str, "%ld", input);
        return str;
    }
    if (DEBUG) {
        printf("Not enough memory!\n");
    }
    return "";
}

int nodeCompare(const void *a, const void *b, void *udata)
{
    const Node *ka = a;
    const Node *kb = b;
    return strcmp(ka->szudzik, kb->szudzik);
}

bool nodeIter(const void *item, void *data)
{
    const Node *keyData = item;
    printf("Visited node {%d,%d} with encoding %s\n", keyData->x, keyData->y, keyData->szudzik);
    return true;
}

uint64_t nodeHash(const void *item, uint64_t seed0, uint64_t seed1)
{
    const Node *Node= item;
    return hashmap_sip(Node->szudzik, strlen(Node->szudzik), seed0, seed1);
}

int manhattan(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// Helper functions for coordinates

char* szudzikEncode(int a, int b)
{
    return longToStr(a >= b? a * a + a + b : a + b * b);
}

void enqueueSurrounding(struct mazeArray *inArray, struct hashmap *visitedNodes, struct hashmap *nodeHeirarchy, minheap *priorityQueue, Node parentNode, Node targetNode)
{

    if (DEBUG) {
        printf("Enqueueing nodes surrounding {%d,%d}:\n", parentNode.x, parentNode.y);
    }

    if (parentNode.x-1 >= 0) {
        char nodeAbove = inArray->arr[parentNode.x-1][parentNode.y];	
        if (nodeAbove == '-') {
            int gScore, hScore;
            hScore = manhattan(parentNode.x-1, parentNode.y, targetNode.x, targetNode.y);
            gScore = parentNode.gScore + 1;
            Node *neighborCoords = malloc(sizeof(Node));
            neighborCoords->x = parentNode.x-1, neighborCoords->y = parentNode.y, neighborCoords->fScore = gScore + hScore, neighborCoords->gScore = gScore, neighborCoords->hScore = hScore, neighborCoords->parentSzudzik = parentNode.szudzik, neighborCoords->szudzik = szudzikEncode(parentNode.x-1, parentNode.y);
            if (!hashmap_get(visitedNodes, neighborCoords)) {
                minheapInsert(*priorityQueue, neighborCoords);
                hashmap_set(nodeHeirarchy, neighborCoords);
                if (DEBUG) {
                    printf("Added {%d,%d} to the queue, with parent {%d,%d}\n", neighborCoords->x, neighborCoords->y, parentNode.x, parentNode.y);
                }
            }
            else if (parentNode.gScore-1 < ((Node*)hashmap_get(nodeHeirarchy, neighborCoords))->gScore) {
                // Updating parent if new parent is closer to origin than old parent
                neighborCoords->gScore = parentNode.gScore + 1;
                neighborCoords->fScore = neighborCoords->hScore + neighborCoords->gScore;
                neighborCoords->parentSzudzik = parentNode.szudzik;
                hashmap_set(nodeHeirarchy, neighborCoords);
            }
        }
    }
    if (parentNode.x+1 < inArray->rows) {
        char nodeBelow = inArray->arr[parentNode.x+1][parentNode.y];
        if (nodeBelow == '-') {
            int gScore, hScore;
            hScore = manhattan(parentNode.x+1, parentNode.y, targetNode.x, targetNode.y);
            gScore = parentNode.gScore + 1;
            Node *neighborCoords = malloc(sizeof(Node));
            neighborCoords->x = parentNode.x+1, neighborCoords->y = parentNode.y, neighborCoords->fScore = gScore + hScore, neighborCoords->gScore = gScore, neighborCoords->hScore = hScore, neighborCoords->parentSzudzik = parentNode.szudzik, neighborCoords->szudzik = szudzikEncode(parentNode.x+1, parentNode.y);
            if (!hashmap_get(visitedNodes, neighborCoords)) {
                minheapInsert(*priorityQueue, neighborCoords);
                hashmap_set(nodeHeirarchy, neighborCoords);
                if (DEBUG) {
                    printf("Added {%d,%d} to the queue, with parent {%d,%d}\n", neighborCoords->x, neighborCoords->y, parentNode.x, parentNode.y);
                }
            }
            else if (parentNode.gScore-1 < ((Node*)hashmap_get(nodeHeirarchy, neighborCoords))->gScore) {
                // Updating parent if new parent is closer to origin than old parent
                neighborCoords->gScore = parentNode.gScore + 1;
                neighborCoords->fScore = neighborCoords->hScore + neighborCoords->gScore;
                neighborCoords->parentSzudzik = parentNode.szudzik;
                hashmap_set(nodeHeirarchy, neighborCoords);
            }

        }
    }
    if (parentNode.y-1 >= 0) {
        char nodeLeft = inArray->arr[parentNode.x][parentNode.y-1];
        if (nodeLeft == '-') {
            int gScore, hScore;
            hScore = manhattan(parentNode.x, parentNode.y-1, targetNode.x, targetNode.y);
            gScore = parentNode.gScore + 1;
            Node *neighborCoords = malloc(sizeof(Node));
            neighborCoords->x = parentNode.x, neighborCoords->y = parentNode.y-1, neighborCoords->fScore = gScore + hScore, neighborCoords->gScore = gScore, neighborCoords->hScore = hScore, neighborCoords->parentSzudzik = parentNode.szudzik, neighborCoords->szudzik = szudzikEncode(parentNode.x, parentNode.y-1);
            if (!hashmap_get(visitedNodes, neighborCoords)) {
                minheapInsert(*priorityQueue, neighborCoords);
                hashmap_set(nodeHeirarchy, neighborCoords);
                if (DEBUG) {
                    printf("Added {%d,%d} to the queue, with parent {%d,%d}\n", neighborCoords->x, neighborCoords->y, parentNode.x, parentNode.y);
                }
            }
            else if (parentNode.gScore-1 < ((Node*)hashmap_get(nodeHeirarchy, neighborCoords))->gScore) {
                // Updating parent if new parent is closer to origin than old parent
                neighborCoords->gScore = parentNode.gScore + 1;
                neighborCoords->fScore = neighborCoords->hScore + neighborCoords->gScore;
                neighborCoords->parentSzudzik = parentNode.szudzik;
                hashmap_set(nodeHeirarchy, neighborCoords);
            }

        }
    }
    if (parentNode.y+1 < inArray->columns) {
        char nodeRight = inArray->arr[parentNode.x][parentNode.y+1];
        if (nodeRight == '-') {
            int gScore, hScore;
            hScore = manhattan(parentNode.x, parentNode.y+1, targetNode.x, targetNode.y);
            gScore = parentNode.gScore + 1;
            Node *neighborCoords = malloc(sizeof(Node));
            neighborCoords->x = parentNode.x, neighborCoords->y = parentNode.y+1, neighborCoords->fScore = gScore + hScore, neighborCoords->gScore = gScore, neighborCoords->hScore = hScore, neighborCoords->parentSzudzik = parentNode.szudzik, neighborCoords->szudzik = szudzikEncode(parentNode.x, parentNode.y+1);
            if (!hashmap_get(visitedNodes, neighborCoords)) {
                minheapInsert(*priorityQueue, neighborCoords);
                hashmap_set(nodeHeirarchy, neighborCoords);
                if (DEBUG) {
                    printf("Added {%d,%d} to the queue, with parent {%d,%d}\n", neighborCoords->x, neighborCoords->y, parentNode.x, parentNode.y);
                }
            }
            else if (parentNode.gScore-1 < ((Node*)hashmap_get(nodeHeirarchy, neighborCoords))->gScore) {
                // Updating parent if new parent is closer to origin than old parent
                neighborCoords->gScore = parentNode.gScore + 1;
                neighborCoords->fScore = neighborCoords->hScore + neighborCoords->gScore;
                neighborCoords->parentSzudzik = parentNode.szudzik;
                hashmap_set(nodeHeirarchy, neighborCoords);
            }

        }
    }
}

bool astar(struct mazeArray* inArray, struct hashmap *visitedNodes, struct hashmap *nodeHeirarchy, minheap *priorityQueue, Node *startingCoords, Node endingCoords)
{
    minheapInsert(*priorityQueue, startingCoords);
    hashmap_set(nodeHeirarchy, startingCoords);
    
    while (!minheapIsEmpty(*priorityQueue)) {
        Node currentNode = minheapFindMin(*priorityQueue);
        minheapDeleteMin(*priorityQueue);
        hashmap_set(visitedNodes, &currentNode);

        if (DEBUG) {
            printf("Current node has szudzikPair {%s}-(%s)\n", currentNode.szudzik, currentNode.parentSzudzik);
        }

        if (!strcmp(currentNode.szudzik, endingCoords.szudzik)) {
            if (DEBUG) {
                printf("The nodes {%d,%d}-(%s) and {%d,%d}-(%s) are equal\n", currentNode.x, currentNode.y, currentNode.szudzik, endingCoords.x, endingCoords.y, endingCoords.szudzik);
            }
            if (DEBUG) {
                printf("\n\nFound path!!!\n\n\n");
            } 

            Node outNode = currentNode;

            while (outNode.parentSzudzik[0] != '\0') {
                coords logCoords = {outNode.x, outNode.y, outNode.szudzik}; 
                if (DEBUG) {
                    printf("Adding {%d,%d} to the stack\n", outNode.x, outNode.y);
                }
                push(&outStack, logCoords); 


                Node searchNode = {0, 0, 0, 0, 0, outNode.parentSzudzik, ""};

                outNode = *((Node*)(hashmap_get(nodeHeirarchy, &searchNode)));
            }

            // Do it one more time for the origin node
            coords logCoords = {outNode.x, outNode.y, outNode.szudzik}; 
            if (DEBUG) {
                printf("Adding {%d,%d} to the stack\n", outNode.x, outNode.y);
            }
            push(&outStack, logCoords); 



            return true;
        }

        if (DEBUG) {
            printf("The node {%d,%d} has the lowest F-score of %d\n", currentNode.x, currentNode.y, currentNode.fScore);
        }


        enqueueSurrounding(inArray, visitedNodes, nodeHeirarchy, priorityQueue, currentNode, endingCoords);
    }

    printf("Minheap is empty\n");
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

        // Creating the hashmaps
        struct hashmap *visitedNodes = hashmap_new(sizeof(Node), 0, 0, 0, nodeHash, nodeCompare, NULL, NULL);
        struct hashmap *nodeHeirarchy = hashmap_new(sizeof(Node), 0, 0, 0, nodeHash, nodeCompare, NULL, NULL);

        // Creating the priority queue (as a minheap)
        minheap priorityQueue = minheapCreate();

        // Creating the maze
        struct mazeArray mazeGrid = loadFile(argv[1]);

        // Finding the starting and ending nodes
        int startingNodeIndex = findPathIndexInArray(mazeGrid.arr[0], mazeGrid.columns);
        int endingNodeIndex = findPathIndexInArray(mazeGrid.arr[mazeGrid.rows - 1], mazeGrid.columns);

        if (DEBUG) {
            printf("Starting node coords: {%d,%d}\n", 0, startingNodeIndex);
            printf("Ending node coords: {%d,%d}\n", mazeGrid.rows - 1, endingNodeIndex);
        }

        Node startingNode = {0, startingNodeIndex, manhattan(0, startingNodeIndex, mazeGrid.rows-1, endingNodeIndex), 0, 0, szudzikEncode(0, startingNodeIndex), ""};
        Node endingCoords = {mazeGrid.rows - 1, endingNodeIndex, 0, 0, 0, szudzikEncode(mazeGrid.rows-1, endingNodeIndex), ""};

        hashmap_set(nodeHeirarchy, &startingNode);

        clock_t begin = clock();

        // Performing the A* search
        if (astar(&mazeGrid, visitedNodes, nodeHeirarchy, &priorityQueue, &startingNode, endingCoords))
        {
            clock_t end = clock();

            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

            // If we have a successful result: 

            char *outPath = argv[1];
            int len = strlen(outPath);
            outPath[len-4] = '\0';
            strncat(outPath, "-solution-astar.txt", 19);

            // Calculate the amount of nodes in the path and output the final solution
            
            int finalPathNodes = 0;
            FILE* solutionFile;
            solutionFile = fopen(outPath, "w");

            fprintf(solutionFile, "The solution to the maze takes the following path:\n");

            // Trace up the output path and push each node into the outStack
            
            bool writing = false;

            while (!isEmpty(&outStack)) {
                if (writing) {
                    fprintf(solutionFile, ", ");
                }
                coords nextCoords = pop(&outStack);
                mazeGrid.arr[nextCoords.x][nextCoords.y] = 'o';
                fprintf(solutionFile, "{%d, %d}", nextCoords.x, nextCoords.y);
                writing = true;
                finalPathNodes++;
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
                strncat(outPath, "-astar.ppm", 10);
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
