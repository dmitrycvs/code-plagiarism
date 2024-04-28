#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

HashMap *createHashMap()
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->entries = (Entry **)calloc(SIZE, sizeof(Entry *));
    return map;
}

int hash(char *key)
{
    int hash = 0;
    while (*key)
    {
        hash += *key;
        key++;
    }
    return hash % SIZE;
}

void insert(HashMap *map, char *key, char *value)
{
    int index = hash(key);
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    if (entry == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (map->entries[index] == NULL)
    {
        map->entries[index] = entry;
    }
    else
    {
        Entry *current = map->entries[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = entry;
    }
}

char *search(HashMap *map, char *key)
{
    int index = hash(key);

    Entry *current = map->entries[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current->value;
        }
        current = current->next;
    }
    return "";
}

void freeHashMap(HashMap *map)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (map->entries[i] != NULL)
            free(map->entries[i]);
    }
    free(map->entries);
    free(map);
}