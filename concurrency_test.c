#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "log.h"
#include "map.h"
#include <time.h>
#include <stdlib.h>
#define BUFFER_SIZE 200

typedef struct buffer
{
    char data[BUFFER_SIZE];
    int status;
} Buffer;

typedef struct
{
    char *key;
    char *value;
    hashmap *map;
} worker_hashmap_struct;

void *worker_put(void *args)
{
    worker_hashmap_struct *real_args = args;
    hashmap_put(real_args->map, real_args->key, real_args->value);
    printf("inserted k: %s -> v: %s\n", real_args->key, real_args->value);

    return NULL;
}
void *worker_get(void *args)
{
    worker_hashmap_struct *real_args = args;
    real_args->value = hashmap_get(real_args->map, real_args->key);
    printf("read k: %s -> v: %s\n", real_args->key, real_args->value);

    return NULL;
}
void *worker_remove(void *args)
{
    worker_hashmap_struct *real_args = args;
    hashmap_remove(real_args->map, real_args->key);
    printf("removed k: %s\n", real_args->key);
    return NULL;
}

int main()
{

    srand(time(NULL));
    log_set_level(LOG_DEBUG);
    hashmap *m = hashmap_create(5);
    char *keys[5] = {"a", "b", "c", "d", "e"};
    char *values[5] = {"1", "2", "3", "4", "5"};

    pthread_t t[10];
    worker_hashmap_struct *args = malloc(sizeof *args);

    int i = 0;

    while (i < 10)
    {
        int op = rand() % 4;
        args->map = m;
        args->key = keys[rand() % 4];
        args->value = values[rand() % 4];

        switch (op)
        {
        case 1:
            pthread_create(&t[i], NULL, worker_put, args);
        case 2:
            pthread_create(&t[i], NULL, worker_get, args);
        }

        i++;
        hashmap_print(m);
    }
    i = 0;
    while (i < 10)
    {
        pthread_join(t[i], NULL);

        i++;
    }
    free(args);
}
