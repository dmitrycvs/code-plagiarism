#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void insertInHashMap(HashMap *map, char *key, char *value)
{
    int index = hash(key);
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    if (entry == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    entry->key = strdup(key);
    entry->value = strdup(value);
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

char *searchInHashMap(HashMap *map, char *key)
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
        Entry *current = map->entries[i];
        while (current != NULL)
        {
            Entry *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(map->entries);
    free(map);
}

void incrementCount(HashMap *Map, char *countName)
{
    char *count = searchInHashMap(Map, countName);
    if (count)
    {
        int countNumber = atoi(count) + 1;
        char *str = (char *)malloc((strlen(count) + 2) * sizeof(char));
        sprintf(str, "%d", countNumber);
        str[strlen(str)] = '\0';

        int index = hash(countName);
        Entry *current = Map->entries[index];
        while (current != NULL)
        {
            if (strcmp(current->key, countName) == 0)
            {
                free(current->value);
                current->value = str;
                return;
            }
            current = current->next;
        }

        insertInHashMap(Map, countName, str);
        free(str);
    }
    else
    {
        insertInHashMap(Map, countName, "1");
    }
}