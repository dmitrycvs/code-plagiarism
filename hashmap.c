#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

typedef struct Entry
{
    char *key;
    char *value;
    struct Entry *next;
} Entry;

typedef struct
{
    Entry **entries;
} HashMap;

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
    entry->key = key;
    entry->value = value;

    map->entries[index] = entry;
}

char *search(HashMap *map, char *key)
{
    int index = hash(key);
    if (index)
        return map->entries[index]->value;

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

int main()
{
    HashMap *myMap = createHashMap();
    insert(myMap, "name", "dimon");
    insert(myMap, "surname", "dimon123");
    printf("%s\n%s\n", search(myMap, "name"), search(myMap, "surname"));
    freeHashMap(myMap);

    return 0;
}