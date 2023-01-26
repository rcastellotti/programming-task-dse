#include <pthread.h>
#ifndef LIST_H
#define LIST_H

typedef struct node Node;

struct node
{
    char *key;
    char *value;
    struct node *next;
};

typedef struct
{
    pthread_rwlock_t lock;
    Node *head;
} List;

List *list_create();
void list_add(List *list, char *key, char *value);
char *list_get(List *list, char *key);
void list_print(List *list);
void list_destroy(List *list);

#endif
