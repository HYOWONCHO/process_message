#include "ipc_com.h"



int test_ipcsvr_main(int argc, char **argv)
{
    int ret = -1;
    ipc_com_t *handle = NULL;
    char buf[1024] = {0xAA, };
    int buflen = 0L;
    int indicator = 0L;
    errno_t err = EOK;

    const char *byebye = "bye";
    

    ipc_xfer_t xfer = {
        .mq = 0,
        .xfer_buf = buf,
        .xfer_len = buflen
    };  

    ret = ipc_com_create((void **)&handle, (void *)&xfer, IPC_TYPE_SERVER);
    if(ret != 0) {
        goto err_done;
    }


    if(handle->open((void *)&xfer) < 0) {
        goto err_done;
    }

    ipc_com_state((void *)handle);

    do {

        memzero_s(xfer.xfer_buf, 1024);

        xfer.xfer_len = handle->attr.mq_msgsize;
        if(ipc_com_recv((void *)&xfer) < 0) {
            err_printf("MQ receive failed (%s)",strerror(errno));
        }

        err = strcasecmp_s((const char *)xfer.xfer_buf, 3, byebye, &indicator) ;
        if(indicator != 0) {
            xfer.xfer_len = strlen(xfer.xfer_buf);
            ipc_com_send((void *)&xfer);
        }

    }while((err == EOK) && (indicator != 0)); 



   

    handle->close(handle);



    RETURN_VAL(0);

err_done:
    RETURN_VAL(ret);
}
