#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

typedef struct node Node;

typedef struct list List;

struct node
{
    char *key;
    char *value;
    struct node *next;
};

struct list
{
    Node *head;
};

List *list_create();
void list_add(List *list, char *key, char *value);
void delete(List *list, char *key);
char *list_get(List *list, char *key);
void list_print(List *list);
void list_destroy(List *list);

#endif
