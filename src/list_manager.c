#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pump_debug.h"
#include "list_manager.h"
#include "safe_str_lib.h"


int list_get_size(list_priv_t *list)
{
    return (int)list->size;
}


list_element_t *list_tail(list_priv_t *list)
{
    return list->tail;
}

list_element_t *list_head(list_priv_t *p)
{
    return p->head;
}

int is_list_head(list_priv_t *list, list_element_t *element)
{
    if(element == list->head) {
        return 1;
    }
    
    return -1;
}

void *list_data(list_element_t *element)
{
    return element->data;
}


list_element_t *list_next(list_element_t *element)
{
    return element->next;
}

void list_initialize(list_priv_t *list, void (*destroy)(void*))
{
    list->size = 0L;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return;
}

void list_destroy(list_priv_t *list)
{
    void *data;

    // remove each element from list
    while(list_get_size(list) > 0) {
        if(list_remove_next(list, NULL, (void **)&data) == 0 &&
                list->destroy  != NULL) {
            list->destroy(data);
        }
    }


    memzero_s((void *)list, sizeof *list);
    return;
}


int list_insert_next(list_priv_t *list, list_element_t *element, const void *data)
{
    list_element_t *new;

    if((new = (list_element_t *)calloc(1, sizeof *new)) == NULL) {
        err_printf("failed to create new element");
        return -1;
    }

    new->data = (void *)data;
    if(element == NULL) {
        // Add a new element on the HEAD of list
        if(list_get_size(list) == 0) {
            list->tail = new;
        }

        new->next = list->head;
        list->head = new;
    }
    else {
        if(element->next == NULL) {
            list->tail = new;
        }

        new->next = element->next;
        element->next = new;
    }

    list->size++;

    return 0;
}

int list_remove_next(list_priv_t *list, list_element_t *element, void **data)
{
    list_element_t *old;

    //check whether list is empty 
    if(list_get_size(list) == 0) {
        err_printf("list empty");
        return -1;
    }

    if(element == NULL) {
        *data = list->head->data;
        old = list->head;
        list->head = list->head->next;

        if(list_get_size(list) == 1) {
            list->tail = NULL;
        }
    }
    else {
        if(element->next == NULL) {
            err_printf(" element is nothing after tail");
            return -1;
        }

        *data = element->data;
        old = element->next;
        element->next = element->next->next;

        if(element->next == NULL) {
            list->tail = element;
        }
    }


    if(old != NULL) {
        free(old);
        old= NULL;
    }

    list->size--;
    return 0;
}

