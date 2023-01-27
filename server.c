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
    log_info("inserted k: %s -> v: %s", real_args->key, real_args->value);

    return NULL;
}
void *worker_get(void *args)
{
    worker_hashmap_struct *real_args = args;
    real_args->value = hashmap_get(real_args->map, real_args->key);
    log_info("read k: %s -> v: %s", real_args->key, real_args->value);

    return NULL;
}
void *worker_remove(void *args)
{
    worker_hashmap_struct *real_args = args;
    hashmap_remove(real_args->map, real_args->key);
    log_info("removed k: %s", real_args->key);

    return NULL;
}

void parse(hashmap *m, char *command)
{
    log_debug("received command: %s", command);
    int i = 0;
    pthread_t t = NULL;

    char arr[3][50];
    char *pch = strtok(command, " ");
    while (pch != NULL)
    {
        strcpy(arr[i], pch);
        i++;
        pch = strtok(NULL, " ");
    }

    worker_hashmap_struct *args = malloc(sizeof *args);
    args->key = arr[1];
    args->map = m;

    if (strcmp(arr[0], "put") == 0)
    {
        args->value = arr[2];
        if (pthread_create(&t, NULL, worker_put, args))
        {
            perror("Cannot create thread\n");
            exit(1);
        }
        if (pthread_join(t, NULL))
        {
            fprintf(stderr, "Cannot join thread\n");
            exit(1);
        }
    }
    else if (strcmp(arr[0], "remove") == 0)
    {
        args->value = arr[2];
        if (pthread_create(&t, NULL, worker_remove, args))
        {
            perror("Cannot create thread\n");
            exit(1);
        }
        if (pthread_join(t, NULL))
        {
            fprintf(stderr, "Cannot join thread\n");
            exit(1);
        }
    }
    else if (strcmp(arr[0], "get") == 0)
    {
        if (pthread_create(&t, NULL, worker_get, args))
        {
            perror("Cannot create thread\n");
            exit(1);
        }
        if (pthread_join(t, NULL))
        {
            fprintf(stderr, "Cannot join thread\n");
            exit(1);
        }
    }
    else if (strcmp(arr[0], "print") == 0)
    {
        hashmap_print(m);
    }
    else
    {
        log_fatal("command not recognized: %s", arr[0]);
    }
}

int main(int argc, char **argv)
{

#ifdef DEBUG
    log_set_level(LOG_DEBUG);
#else
    log_set_level(LOG_INFO);
#endif
    if (argc < 2)
    {
        log_fatal("error: usage ./server <size>");
        exit(1);
    }
    int shmid;
    Buffer *shm;
    key_t key;
    hashmap *m = hashmap_create(atoi(argv[1]));

    key = ftok("shared_mem_chat", 99);

    if ((shmid = shmget(key, sizeof(Buffer), IPC_CREAT | 0666)) < 0)
    {
        perror("error: shmget");
        exit(1);
    }

    // https://stackoverflow.com/a/70727205
    if ((shm = shmat(shmid, NULL, 0)) == (void *)-1)
    {
        perror("error:shmat");
        exit(1);
    }

    shm = (Buffer *)shmat(shmid, NULL, 0);
    while (1)
    {
        while (shm->status == 1 && strlen(shm->data) > 1)
        {
            shm->data[strlen(shm->data) - 1] = '\0'; // just to fix the formatting in the output
            parse(m, shm->data);
            shm->status = 0;
        }
    }

    if (shmdt((void *)shm) == -1)
    {
        perror("error: shmdt");
        exit(1);
    };
    return 0;
}
