#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 200

typedef struct buffer
{
    char data[BUFFER_SIZE];
    int status;
} Buffer;

int main()

{
    printf("usage:\n");
    printf("  send a command in this form: <operation> <key> <value>\n");
    printf("  for example: `insert distsys 10`\n");
    int shmid;
    Buffer *shm;
    key_t key;

    key = ftok("shared_mem_chat", 99);

    if ((shmid = shmget(key, sizeof(Buffer), IPC_CREAT | 0666)) < 0)
    {
        perror("error: shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (void *)-1)
    {
        perror("error:shmat");
        exit(1);
    }

    shm->status = 1;
    while (1)
    {
        while (shm->status != 1)
            continue;
        printf("> ");
        fgets(shm->data, BUFFER_SIZE, stdin);
        shm->status = 1;
    }

    if (shmdt((void *)shm) == -1)
    {
        perror("error: shmdt");
        exit(1);
    };
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("error: shmctl");
        exit(1);
    };
    return 0;
}
