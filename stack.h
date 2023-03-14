#ifndef STACK_H 
#define STACK_H 

#include <stdbool.h>

typedef struct coords
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

void push (struct node **head, coords nodeCoords);
coords pop(struct node **head);
coords peek(struct node **head);
bool isEmpty(struct node **head);

#endif
