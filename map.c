#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "log.h"
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int hash(char *key)
{
    int hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASHMAP_SIZE;
}

hashmap *hashmap_create(int size)
{
    hashmap *m = malloc(sizeof(hashmap));
    m->size = size;
    for (int i = 0; i < size; i++)
    {
        bucket *new_bucket = (struct bucket *)malloc(sizeof(bucket));
        new_bucket->key = "dead";
        new_bucket->value = "dead";
        new_bucket->next = m->first;
        m->first = new_bucket;
    }
    m->last = NULL;
    return m;
}

void hashmap_put(struct hashmap *map, char *key, char *value)
{
    int h = hash(key) % map->size;
    pthread_rwlock_wrlock(&rwlock);
    bucket *counter = map->first;
    log_debug("inserting  %s ->  %s current->key is: %s ", key, value, counter->key);

    for (int cont = 0; cont < h; cont++)
    {
        counter = counter->next;
    }
    // collisions and empty list
    if (strcmp(counter->key, "dead") == 0 || strcmp(counter->key, key) == 0)
    {
        counter->key = key;
        counter->value = value;
    }
    else
    {
        bucket *new_bucket = (struct bucket *)malloc(sizeof(bucket));
        new_bucket->key = key;
        new_bucket->value = value;
        new_bucket->next = counter->bucketlist;
        counter->bucketlist = new_bucket;
    }
    pthread_rwlock_unlock(&rwlock);
}

char *hashmap_get(struct hashmap *map, char *key)
{
    bucket *counter = map->first;
    int h = hash(key) % map->size;
    pthread_rwlock_rdlock(&rwlock);
    for (int cont = 0; cont < h; cont++)
    {
        counter = counter->next;
    }
    pthread_rwlock_unlock(&rwlock);

    return counter->value;
}

void hashmap_remove(struct hashmap *map, char *key)
{
    bucket *counter = map->first;

    int h = hash(key) % map->size;
    pthread_rwlock_wrlock(&rwlock);

    for (int cont = 0; cont < h; cont++)
    {
        counter = counter->next;
    }
    counter->key = "dead";
    counter->value = "dead";
    pthread_rwlock_unlock(&rwlock);
}

void hashmap_print(struct hashmap *map)
{

    bucket *counter = map->first;

    int conteggio = 0;
    while (counter != NULL)
    {

        printf("%i :: ", conteggio);
        printf("%s -> ", counter->key);

        printf("%s ", counter->value);
        bucket *counterlist = counter->bucketlist;
        while (counterlist != NULL)
        {
            printf(" :: ");
            printf("%s -> ", counterlist->key);
            printf("%s", counterlist->value);
            counterlist = counterlist->next;
        }
        printf("\n");
        counter = counter->next;
        conteggio++;
    }
}
