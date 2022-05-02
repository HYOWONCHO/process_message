#pragma once


typedef int (*cb_list_match)(void*, void *);
typedef void (*cb_list_destroy)(void*, void *);

/**
 * @brief Struct for linked-list structure
 */
typedef struct _t_list_element {
    void                    *data;  /**< Private data for list structure */
    struct _t_list_element  *next;
}list_element_t;


/**
 * @brief Linked list data structure
 */
typedef struct _t_list_priv {
    int     size;
    int     (*match)(const void *key1, const void *key2);
    void    (*destroy)(void *priv);

    list_element_t      *head;
    list_element_t      *tail;
}list_priv_t;

/**
 * list_initialize -  Initializes the linked list of type linkedList.
 *  All linked lists must be initialized before any other operations
 *  can be ran on them. The destroy argument is put here as a way to
 *  free allocated data when list_destroy is used
 *
 *  @param list:
 *  @param destroy:
 *
 *  @return None
 */
void list_initialize(list_priv_t *list, void (*destroy)(void*));

/**
 * list_destroy - Will destroy a list which is passed to it in the
 *  form of a pointer to a linkedList pointer. When a list has been
 *  destroyed, no more operations may be carried out unless the
 *  list_initialize function is called again. The list_destroy
 *  function removes all elements from the list, and calls the
 *  function passed in as destroy during the list_initialize, once
 *  for every element during the destroy process. Destroy will not
 *  be performed if the destroy function was passed in as NULL.
 *
 *  @param list:
 *
 *  @return none
 */
void list_destroy(list_priv_t *list);

/** 
 * list_insert_next -  Adds an element to the list (passed as "list")
 *  AFTER the element (passed as "element"). If "element" is NULL,
 *  then the new element will be added as the head of the list.
 *  The new element will contain a pointer to "data". This means the
 *  memory referenced by "data" needs to remain valid for the lifetime
 *  of the element in the list. It is the responsibility of the developer
 *  to manage this storage. 
 *
 *  @param list:
 *  @param element:
 *  @param data:
 *
 *  @return On success, return the 0, otherwise return the -1
 */
int list_insert_next(list_priv_t *list, list_element_t *element, const void *data);


/** 
 * list_remove_next -  Removes the element form the list (passed as "list")
 *  AFTER the element (passed as "element"). If "element is NULL, then
 *  the element which represents the head of the list will be removed.
 *  When this function returns, "data" will still point to the data which
 *  was stored in the element which has been removed. It is the responsibility
 *  of the programmer to manage this storage.
 *
 *  @param list:
 *  @param element:
 *  @param data:
 *
 *  @return On success, return the 0, otherwise return the -1
 */
int list_remove_next(list_priv_t *list, list_element_t *element, void **data);

list_element_t *list_head(list_priv_t *p);
int list_get_size(list_priv_t *list);
list_element_t *list_next(list_element_t *t);
list_element_t *list_tail(list_priv_t *list);
void *list_data(list_element_t *element);
