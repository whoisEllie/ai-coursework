#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fileloader.h"

#define DEBUG false

// Helpers

char** createCharArray(int m, int n)
{
	char* values = calloc(m*n, sizeof(char));
	char** rows = malloc(m*sizeof(char*));
	for (int i=0; i<m; ++i) {
		rows[i] = values + i*n;
	}
	return rows;
}

// Reading from files

struct mazeArray loadFile(char* filePath)
{
	// loading file
	FILE* fptr;
	char c;

	if (false) {
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
