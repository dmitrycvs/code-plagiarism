#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
}

void insertInList(Node **headRef, int data)
{
    Node *newNode = createNode(data);
    if (*headRef == NULL)
    {
        *headRef = newNode;
        return;
    }
    Node *tmp = *headRef;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
}

void displayList(Node *head)
{
    while (head != NULL)
    {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

void deleteList(Node **headref)
{
    Node *currentNode = *headref;
    Node *nextNode = NULL;

    while (currentNode != NULL)
    {
        nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    *headref = NULL;
}

void changeThePointerOfList(Node **headref)
{
    Node *nextNode = (*headref)->next;
    *headref = nextNode;
}