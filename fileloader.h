#ifndef FILELOADER_H
#define FILELOADER_H

struct mazeArray 
{
	  char** arr;
    int rows; 
	  int columns;
	  int nodeAmount;
};

struct mazeArray loadFile(char* filePath);

#endif
