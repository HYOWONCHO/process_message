#include "thread_mgm.h"
#include "pump_msg_macro.h"


static void _print_recording_list(list_element_t *l)
{
    list_element_t *pos = NULL;
    pos = list_head(l);
    for(int i = 0; i < list_get_size(l); i++) {
        info_printf("Data : %s ", (char *)pos->data);
        pos = list_next(pos);
    }
}

void *xfer_start(void *priv)
{
    thread_mgm_t *handle = (thread_mgm_t *)priv;

    //printf("handle address: %p", handle);

    debug_printf("wait capture thread wait ~~~");
    COND_WAIT(handle);

    debug_printf("wake-up xfer thread !!!");

    debug_printf("Handle list: %p", handle->s);
    _print_recording_list(handle->s);


    // TO DO: Send the recording list to the Host
     

    return NULL;

}
