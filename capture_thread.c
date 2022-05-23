#include "thread_mgm.h"
#include "pump_msg_macro.h"
#include "file_mgm.h"


static int get_recording_list(void *priv)
{
    list_element_t *l = NULL;
    record_file_t *p =(record_file_t *)priv;

    if(record_stream_list(p, 1) < 0) {
        return EFAIL;

    }


    return EOK;
}

void *capture_start(void *priv)
{
    int ret = EOK;
    record_file_t p;
    thread_mgm_t *handle = NULL;
    handle = (thread_mgm_t *)priv;
    //printf("handle address: %p", handle);

    memzero_s((void *)&p, sizeof p);

    if(file_mgm_init(&p) != EOK) {
        err_printf("File Manager Module initalize fail");
        goto err_done;
    }

    ret = get_recording_list((void *)&p);
    if(ret != EOK) {
        err_printf("Recoding list warning");
        goto err_done;
    }


    handle->s = p.list;

    debug_printf("Handle list: %p (%p)and size : %d and size : %d", 
            handle->s,  p.list,
            list_get_size(p.list), list_get_size(handle->s));

    debug_printf("signal to xfer~~~");

    COND_SIGNAL(handle);




    // TO DO : Send file list





err_done:

    return NULL;


    
}
