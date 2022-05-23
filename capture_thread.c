#include "thread_mgm.h"
#include "pump_msg_macro.h"

void *capture_start(void *priv)
{
    thread_mgm_t *handle = NULL;
    handle = (thread_mgm_t *)priv;
    printf("handle address: %p", handle);


    debug_printf("signal to xfer~~~");
    sleep(3);
    COND_SIGNAL(handle);



    return NULL;


    
}
