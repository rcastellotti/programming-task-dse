#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define HASHMAP_SIZE 10

typedef struct bucket bucket;
typedef struct hashmap hashmap;
typedef struct list list;

struct bucket
{
    List *bucketlist;
};

struct hashmap
{
    int size;
    List *buckets[];
};

unsigned int hash(char *key);
hashmap *hashmap_create(int size);
void hashmap_put(hashmap *map, char *key, char *value);
char *hashmap_get(hashmap *map, char *key);
void hashmap_remove(hashmap *map, char *key);
void hashmap_print(hashmap *map);

