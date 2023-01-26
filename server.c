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



void parse(hashmap *m, char *command)
{
    log_debug("received command: %s", command);
    int i = 0;
    char arr[3][50];
    char *pch = strtok(command, " ");
    while (pch != NULL)
    {
        strcpy(arr[i], pch);
        i++;
        pch = strtok(NULL, " ");
    }

    char *key = arr[1];

    if (strcmp(arr[0], "insert") == 0)
    {
        char *value = arr[2];
        hashmap_put(m, strdup(key), strdup(value));
        log_info("inserted k: %s -> v: %s", key, value);
    }
    else if (strcmp(arr[0], "delete") == 0)
    {
        hashmap_remove(m, key);
        log_info("removed k: %s", key);
    }
    else if (strcmp(arr[0], "read") == 0)
    {
        char *res = hashmap_get(m, key);
        log_info("read k: %s -> v: %s", key, res);
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
