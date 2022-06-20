#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>



#include "list.h"


typedef struct {
    char name[255];
    int age;
}person_info_t;

struct object_list_t {
    void *prev;
    struct list_head list;
};


static void *make_person_object(int age, char *name)
{
    int n_len = 0L;
    person_info_t *person;

    person = calloc(1, sizeof *person);
    if(person == NULL) {
        return NULL;
    }

    n_len = strlen(name);
    strncpy(person->name, name, n_len);
    person->name[n_len]  = '\0';
    person->age = age;

    return (void *)person;
}


int object_add_node(void *prev, struct list_head *head)
{
    struct object_list_t *obj;

    if(prev == NULL) {
        printf("invalid argument \n");
        return -1;
    }

    obj = calloc(1, sizeof(struct object_list_t)); 
    if(obj == NULL) {
        printf("memory failed (%s) \n", strerror(errno));
        return -1;
    }

    obj->prev = prev;
    INIT_LIST_HEAD(&obj->list);
    list_add(&obj->list, head);

    printf("obj: %p (list : %p) \n", obj, &obj->list);

    return 0;
}

int object_numof_node(struct list_head *head)
{
    int num = 0L;
    struct list_head *iter;
    struct object_list_t *ptr;
    person_info_t *person;
    list_for_each(iter, head)  {
        num++;
        ptr = list_entry(iter, struct object_list_t, list);
        printf("%s iter: %p ptr : %p (next : %p , prev:%p) (prev:%p)\n", 
                __func__,
                iter, ptr, 
                ptr->list.next, ptr->list.prev,
                ptr->prev);
        person = (person_info_t *)ptr->prev;
    }

    printf("\n\n\n");
    return num;
}

void object_delete_all_node(struct list_head *head)
{

    
    struct list_head *iter;
    struct object_list_t *ptr;

    int num_of_node = object_numof_node(head);

    
    //for(int i = 0; i < num_of_node; i++) {

re_doing:
    list_for_each(iter, head)  {
        ptr = list_entry(iter, struct object_list_t, list);
        printf("iter: %p ptr : %p (next : %p , prev:%p) (prev:%p)\n", 
                iter, ptr, 
                ptr->list.next, ptr->list.prev,
                ptr->prev);
        list_del(&ptr->list);
        free(ptr);
        goto re_doing;
    }
    //}

    return;
}


int main(int argc, char **argv)
{
    person_info_t *person;
    char *name[] = {
        "jax" , "soopi", "viago", NULL
    };

    LIST_HEAD(objhead);
    printf("head address: %p %p %p \n", &objhead, objhead.prev, objhead.next);

    for ( int i = 0; i < (sizeof name / sizeof *name) - 1; i++) {
        printf("%d %s \n", i+30, name[i]);

        person = make_person_object(i+30, name[i]);
       
        printf(" [%d] = %p \n", i, person);

        object_add_node((void *)person, &objhead);
    }

    object_numof_node(&objhead);

    sleep(3);


    object_delete_all_node(&objhead);

    return 0;
}
