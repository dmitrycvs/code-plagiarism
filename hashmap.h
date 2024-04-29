#ifndef HASHMAP_H
#define HASHMAP_H

#define SIZE 10

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

HashMap *createHashMap();
int hash(char *key);
void insertInHashMap(HashMap *map, char *key, char *value);
char *searchInHashMap(HashMap *map, char *key);
void freeHashMap(HashMap *map);
void incrementCount(HashMap *Map, char *countName);

#endif /* HASHMAP_H */
