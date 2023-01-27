#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "list.h"
#include "log.h"
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

unsigned int hash(char *key)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

hashmap *hashmap_create(int size)
{

    hashmap *map = malloc(size * sizeof(List ));
    map->size = size;
    for (int i = 0; i < map->size; i++)
    {
        map->buckets[i] = list_create();
        list_add(map->buckets[i], "dead", "dead");
    }
    return map;
}

void hashmap_put(struct hashmap *map, char *key, char *value)
{
    unsigned int h = hash(key) % map->size;
    pthread_rwlock_wrlock(&map->buckets[h]->lock);
    if (strcmp(map->buckets[h]->head->key, "dead") == 0)
    {
        map->buckets[h]->head->key = key;
        map->buckets[h]->head->value = value;
        return;
    }

    list_add(map->buckets[h], key, value);
    pthread_rwlock_unlock(&map->buckets[h]->lock);
}

char *hashmap_get(struct hashmap *map, char *key)
{
    int h = hash(key) % map->size;
    pthread_rwlock_rdlock(&map->buckets[h]->lock);
    // the key is in this bucket, we only have to find it
    char *value = list_get(map->buckets[h], key);
    pthread_rwlock_unlock(&map->buckets[h]->lock);
    return value;
}

void hashmap_remove(struct hashmap *map, char *key)
{
    int h = hash(key) % map->size;
    pthread_rwlock_wrlock(&map->buckets[h]->lock);
    list_destroy(map->buckets[h]);
    map->buckets[h] = list_create();
    list_add(map->buckets[h], "dead", "dead");
    pthread_rwlock_unlock(&map->buckets[h]->lock);
}

void hashmap_print(struct hashmap *map)
{
    for (int i = 0; i < map->size; i++)
    {
        printf("%i :: ", i);
        list_print(map->buckets[i]);    
        printf("\n");
    }
}
