#include "thread_mgm.h"
#include "pump_msg_macro.h"



void *xfer_start(void *priv)
{
    thread_mgm_t *handle = (thread_mgm_t *)priv;

    printf("handle address: %p", handle);

    debug_printf("wait capture thread wait ~~~");
    COND_WAIT(handle);

    debug_printf("wake-up xfer thread !!!");


    return NULL;

}
