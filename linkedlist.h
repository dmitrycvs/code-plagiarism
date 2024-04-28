#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "hashmap.h"

typedef struct Node
{
    HashMap *data;
    struct Node *next;
} Node;

Node *createNode(HashMap *data);
void insertInList(Node **headRef, HashMap *data);
void deleteList(Node **headRef);
void changeThePointerOfList(Node **headRef);

#endif /* LINKED_LIST_H */
