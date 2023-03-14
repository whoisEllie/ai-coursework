#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_DEBUG false 

// Stack implementation
 
void push (struct node **head, coords nodeCoords){
    struct node *ptr = (struct node*)malloc(sizeof(struct node));

    if(*head == NULL){
        ptr->nodeCoords = nodeCoords;
        ptr->next = NULL;
        *head = ptr;
    }
    else{
        ptr->nodeCoords = nodeCoords;
        ptr->next = *head;
        *head = ptr;
    }
}

coords pop(struct node **head){
    coords item;
    struct node *ptr;
    if (head == NULL)
    {
        if (STACK_DEBUG) {
            printf("Underflow State: can't remove any item\n");
        }
    }
    else
    {
        item = (*head)->nodeCoords;
        ptr = *head;
        *head = (*head)->next;
        free(ptr);
        if (STACK_DEBUG) {
            printf("{%d,%d} is popped out of the stack\n", item.x, item.y);
        }
        return item;
    }
		coords emptyCoords = {0, 0};
    return emptyCoords;
}

coords peek(struct node **head){
    coords x = (*head)->nodeCoords;
    if (STACK_DEBUG) {
        printf("{%d,%d} is the top most element of the stack\n", x.x, x.y);
    }
    return x;
}

bool isEmpty(struct node **head){
    if(*head == NULL){
        if (STACK_DEBUG) {
            printf("Stack is empty: Underflow State\n");
        }
        return true;
    }
    if (STACK_DEBUG) {
        printf("Stack is not empty\n");
    }
    return false;
}

