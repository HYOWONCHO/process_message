#include <stdio.h>
#include <stdlib.h>

#include "list_manager.h"
#include "pump_debug.h"


int list_mgm_test(void)
{
    int i = 0L;

    list_element_t *pos = NULL;

    list_priv_t list;

    list_initialize(&list, NULL);
    
    list_insert_next(&list, NULL, "one");
    pos = list_head(&list);
    
    list_insert_next(&list, pos, "two"); 
    pos = list_next(pos);

    list_insert_next(&list, pos, "three");
    pos = list_next(pos);
    
    /*-------------------------------------------------------------
     *Iterate through the list and output the data in each element
     *-----------------------------------------------------------*/    
    pos = list_head(&list);

    i = 0;
    for (i; i < list_get_size(&list); i++)
    {
        info_printf("Data is : %s.\n", (char*)pos->data);
        pos = list_next(pos);
    }

    
    info_printf ("Size of list BEFORE deleting an element : %d.\n", 
             list_get_size(&list));
    
    /* Remove the node after head */
    pos = list_head(&list);
    list_remove_next(&list, pos, &pos->data);    
    
    info_printf ("Size of list AFTER deleting an element : %d.\n",
             list_get_size(&list));
    /* We have had enough fun now, time to destroy the list*/    
    list_destroy(&list); 
        
    return 0;
}
