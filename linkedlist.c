#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

Node *createNode(HashMap *data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;

    return newNode;
}

void insertInList(Node **headRef, HashMap *data)
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

void deleteList(Node **headref)
{
    Node *currentNode = *headref;
    Node *nextNode = NULL;

    while (currentNode != NULL)
    {
        nextNode = currentNode->next;
        freeHashMap(currentNode->data);
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

int main()
{
    // Create an empty linked list
    Node *head = NULL;

    // Create hashmaps
    HashMap *map1 = createHashMap();
    HashMap *map2 = createHashMap();
    HashMap *map3 = createHashMap();

    // Insert hashmaps into the linked list
    insertInList(&head, map1);
    insertInList(&head, map2);
    insertInList(&head, map3);

    // Insert key-value pairs into each hashmap
    insertInHashMap(map1, "key1", "value1");
    insertInHashMap(map2, "key2", "value2");
    insertInHashMap(map3, "key3", "value3");

    // Search for values in each hashmap
    printf("Value for key 'key1': %s\n", searchInHashMap(head->data, "key1"));
    printf("Value for key 'key2': %s\n", searchInHashMap(head->data, "key2"));
    printf("Value for key 'key3': %s\n", searchInHashMap(head->data, "key3"));
    // printf("Value for key 'key4' in empty hashmap: %s\n", searchInHashMap(createHashMap(), "key4")); // Key not found

    // Delete the linked list
    deleteList(&head);

    return 0;
}
