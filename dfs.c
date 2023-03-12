#include "hashmap.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define DEBUG true 
#define OUTPUT true
#define PATH "./maze-Easy.txt" 

// Structs

struct mazeArray 
{
	char** arr;
	int rows; 
	int columns;
	int nodeAmount;
};

typedef struct
{
	int x;
	int y;
  char* szudzik;
} coords;

struct node
{
	coords nodeCoords;
	struct node* next;
};

struct node *head;

// Stack implementation

void push (coords nodeCoords){
    struct node *ptr = (struct node*)malloc(sizeof(struct node));

    if(head == NULL){
        ptr->nodeCoords = nodeCoords;
        ptr->next = NULL;
        head = ptr;
    }
    else{
        ptr->nodeCoords = nodeCoords;
        ptr->next = head;
        head=ptr;
    }
}

coords pop(){
    coords item;
    struct node *ptr;
    if (head == NULL)
    {
        if (DEBUG) {
            printf("Underflow State: can't remove any item\n");
        }
    }
    else
    {
        item = head->nodeCoords;
        ptr = head;
        head = head->next;
        free(ptr);
        if (DEBUG) {
            printf("{%d,%d} is popped out of the stack\n", item.x, item.y);
        }
        return item;
    }
		coords emptyCoords = {0, 0};
    return emptyCoords;
}

coords peek(){
    coords x = head->nodeCoords;
    if (DEBUG) {
        printf("{%d,%d} is the top most element of the stack\n", x.x, x.y);
    }
    return x;
}

bool isEmpty(){
    if(head == NULL){
        if (DEBUG) {
            printf("Stack is empty: Underflow State\n");
        }
        return true;
    }
    if (DEBUG) {
        printf("Stack is not empty\n");
    }
    return false;
}

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

// Helper functions for arrays 

char** createCharArray(int m, int n)
{
	char* values = calloc(m*n, sizeof(char));
	char** rows = malloc(m*sizeof(char*));
	for (int i=0; i<m; ++i) {
		rows[i] = values + i*n;
	}
	return rows;
}

int findPathIndexInArray(char* inArray, int arraySize)
{
	int i;
	int foundIndex = arraySize+1;

	for (i=0; i<arraySize; i++) {
		if (inArray[i] == '-') {
			foundIndex = i;	
			break;
		}	
	}

	return foundIndex;
}

// Reading from files

struct mazeArray loadFile(char* filePath)
{
	// loading file
	FILE* fptr;
	char c;

	if (DEBUG) {
		// opening file
		fptr = fopen(filePath, "r");
	
		// displaying maze for debug
		while ((c = fgetc(fptr)) != EOF) {
	 	printf("%c", c);
		}	

		fclose(fptr);
	}

	// determining width and height of maze

	fptr = fopen(filePath, "r");

	int i = 0;
	int width = 0;
	int height = 0;
	int numberOfVertices = 0;

	while ((c = fgetc(fptr)) != EOF) {
		if (c == '\n')
		{
			if (width != i && width == 0) {
				width = i; // Updating width if i is different and the width is non-0
			}
			if (i != 0) {
				height++; // Updating the height if there is at least one element present on the line
			}
			i = 0;
		}
		else if (c == '-') 
		{
			i++;
		}
		else if (c == '#')
		{
			i++;
		}
	}

	fclose(fptr);

  if (DEBUG) {
	  printf("Final width is %d\n", width);
	  printf("Final height is %d\n", height);
  }

	// creating array

	char** dataArray = createCharArray(height, width);

	int x = 0;
	int y = 0;

	// populating array with characters

	fptr = fopen(filePath, "r");

	while ((c = fgetc(fptr)) != EOF) {
		if (c == '\n') //newline 
		{
			y++;
			x = 0;
		}
		else if (c == '-') 
		{
			dataArray[y][x] = c;
			x++;
			numberOfVertices++;
		}
		else if (c == '#')
		{
			dataArray[y][x] = c;
			x++;
		}
	}

	fclose(fptr);

  if (DEBUG) {
	  printf("Number of vertices: %d\n", numberOfVertices);
  }

	if (DEBUG) {
		int j;

		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				printf("%c", dataArray[i][j]);
			}
			printf("\n");
		}
	}

	struct mazeArray returnData = { dataArray, height, width, numberOfVertices};

	return returnData;
}

// DFS implementation

void pushNextNeighborToStack(struct mazeArray *inArray, struct hashmap *visitedNodes, coords inCoords)
{
	if (inCoords.x-1 >= 0) {
		char nodeAbove = inArray->arr[inCoords.x-1][inCoords.y];	
		if (nodeAbove == '-') {
			coords neighborCoords = {inCoords.x-1, inCoords.y, szudzikEncode(inCoords.x-1, inCoords.y)};
      if (!hashmap_get(visitedNodes, &neighborCoords)) {
        push(neighborCoords);
        return;
      }
		}
	}
  if (inCoords.x+1 < inArray->rows) {
		char nodeBelow = inArray->arr[inCoords.x+1][inCoords.y];
		if (nodeBelow == '-') {
			coords neighborCoords = {inCoords.x+1, inCoords.y, szudzikEncode(inCoords.x+1, inCoords.y)};
      if (!hashmap_get(visitedNodes, &neighborCoords)) {
        push(neighborCoords);
        return;
      }
		}
	}
  if (inCoords.y-1 >= 0) {
		char nodeLeft = inArray->arr[inCoords.x][inCoords.y-1];
		if (nodeLeft == '-') {
			coords neighborCoords = {inCoords.x, inCoords.y-1, szudzikEncode(inCoords.x, inCoords.y-1)};
      if (!hashmap_get(visitedNodes, &neighborCoords)) {
        push(neighborCoords);
        return;
      }
		}
	}
  if (inCoords.y+1 < inArray->columns) {
		char nodeRight = inArray->arr[inCoords.x][inCoords.y+1];
		if (nodeRight == '-') {
			coords neighborCoords = {inCoords.x, inCoords.y+1, szudzikEncode(inCoords.x, inCoords.y+1)};
      if (!hashmap_get(visitedNodes, &neighborCoords)) {
        push(neighborCoords);
        return;
      }
		}
	}

  if (DEBUG) {
    printf("Found no new neighbors for {%d,%d}\n", inCoords.x, inCoords.y);
  }
  pop();
}

bool depthFirstSearch(struct mazeArray *inArray, struct hashmap *visitedNodes, coords startingCoords, coords endingCoords)
{
	push(startingCoords);


	while (!isEmpty()) {
		coords currentNode = peek();
    hashmap_set(visitedNodes, &currentNode);

    if (currentNode.szudzik == endingCoords.szudzik) 
    {
        printf("%s and %s are equal\n", currentNode.szudzik, endingCoords.szudzik);
        if (DEBUG) {
            printf("Found a path!!!");
        }
        return true;
    }

    pushNextNeighborToStack(inArray, visitedNodes, currentNode);
	}

  return false;
}

int main()
{
  struct hashmap *visitedNodes = hashmap_new(sizeof(coords), 0, 0, 0, coordsHash, coordsCompare, NULL, NULL);
    
  /*hashmap_set(visitedNodes, &(coords){ .x=4, .y=13, .szudzik=szudzikEncode(4, 13)});
  hashmap_set(visitedNodes, &(coords){ .x=15, .y=3, .szudzik=szudzikEncode(15, 3) });
  hashmap_set(visitedNodes, &(coords){ .x=5, .y=17, .szudzik=szudzikEncode(5, 17) });
  hashmap_set(visitedNodes, &(coords){ .x=315, .y=18, .szudzik=szudzikEncode(315, 18) });

  hashmap_scan(visitedNodes, coordsIter, NULL);

  coords *outCoords;

  outCoords = hashmap_get(visitedNodes, &(coords){ .szudzik=szudzikEncode(5, 13) });
  printf("%s\n", outCoords?"exists":"not exists");
  //printf("Found {%d,%d}\n", outCoords->x, outCoords->y);

  return 0;*/

	struct mazeArray mazeGrid = loadFile(PATH);

	int startingNodeIndex = findPathIndexInArray(mazeGrid.arr[0], mazeGrid.columns);
	int endingNodeIndex = findPathIndexInArray(mazeGrid.arr[mazeGrid.rows - 1], mazeGrid.columns);

	if (DEBUG) {
		printf("Starting node coords: {%d,%d}\n", 0, startingNodeIndex);
		printf("Ending node coords: {%d,%d}\n", mazeGrid.rows - 1, endingNodeIndex);
	}

  coords startingCoords = {0, startingNodeIndex, szudzikEncode(0, startingNodeIndex)};
  coords endingCoords = {mazeGrid.rows - 1, endingNodeIndex, szudzikEncode(mazeGrid.rows-1, endingNodeIndex)};
  

	depthFirstSearch(&mazeGrid, visitedNodes, startingCoords, endingCoords);

  if (OUTPUT) {
    while (!isEmpty()) {
      coords nextCoords = pop(); 
      mazeGrid.arr[nextCoords.x][nextCoords.y] = 'o';
      printf("Added a visited node\n");
    }

    FILE* outFile;
    outFile = fopen("./out.ppm", "w");

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
}
