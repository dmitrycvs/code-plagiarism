#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *createNode(int data);
void insertInList(Node **headRef, int data);
void displayList(Node *head);
void deleteList(Node **headRef);
void changeThePointerOfList(Node **headRef);

#endif /* LINKED_LIST_H */
