#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>
List *list_create()
{
    List *list = malloc(sizeof(List));
    if (!list)
    {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void list_print(List *list)
{
    Node *current = list->head;
    if (list->head == NULL)
        return;
    while (current != NULL)
    {
        printf("%s -> %s :: ", current->key, current->value);
        current = current->next;
    }
}
void list_add(List *list, char *key, char *value)
{
    Node *current = NULL;
    if (list->head == NULL)
    {
        Node *newNode = malloc(sizeof(Node));
        newNode->key = key;
        newNode->value = value;
        newNode->next = NULL;
        list->head = newNode;
    }
    else
    {
        current = list->head;
        // handling duplicates
        while (current->next != NULL || strcmp(current->key, key) == 0)
        {
            current->value = value;
            return;
            current = current->next;
        }
        Node *newNode = malloc(sizeof(Node));
        newNode->key = key;
        newNode->value = value;
        newNode->next = NULL;
        current->next = newNode;
    }
}

void delete(List *list, char *key)
{
    Node *current = list->head;
    Node *previous = current;
    while (current != NULL)
    {
        if (current->key == key)
        {
            previous->next = current->next;
            if (current == list->head)
                list->head = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}
char *list_get(List *list, char *key)
{
    Node *current = list->head;
    Node *previous = current;
    while (current != NULL)
    {
        if (current->key == key)
        {
            previous->next = current->next;
            if (current == list->head)
                list->head = current->next;
            return current->value;
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

void list_destroy(List *list)
{
    Node *current = list->head;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
