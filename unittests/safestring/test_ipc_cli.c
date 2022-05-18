#include "ipc_com.h"


static char* _get_line(void) 
{
#if !defined(__OWN_FUNC___)
    char *linep = NULL;
    size_t len;
    ssize_t read = 0;

    getline(&linep, &len, stdin);
#else
    char *line = (char *)calloc(1, 1024);
    char *linep;
    size_t lenmax = 1024;
    size_t len;
    int c; 


    linep = line;
    len = lenmax;

    if(line == NULL) {
        err_printf("Mem alloc fail (%s)", strerror(errno));
        return NULL;
    }


    for(;;) {
        c = fgetc();
        if( c == EOF ) {
            break;
        }

        if( --len == 0 ) {
            len = lenmax;
            char *linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                linep = NULL;
                return NULL;
            }

            line = linen + (line - linep);
            linep = linen;

        }

        if((*line++ = c) == '\n') {
            break;
        }
    }

    *line = '\0';
#endif
    return linep;
}


int test_ipccli_main(int argc, char **argv)
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

    ret = ipc_com_create((void **)&handle, (void *)&xfer, IPC_TYPE_CLIENT);
    if(ret != 0) {
        goto err_done;
    }


    if(handle->open((void *)&xfer) < 0) {
        goto err_done;
    }

    ipc_com_state((void *)handle);

    do {

        printf("Message: ");


        memzero_s(xfer.xfer_buf, 1024);

        ipc_com_send((void *)&xfer);

        xfer.xfer_len = handle->attr.mq_msgsize;
        if(ipc_com_recv((void *)&xfer) < 0) {
            err_printf("MQ receive failed (%s)",strerror(errno));
        }

        err = strcasecmp_s((const char *)xfer.xfer_buf, 3, byebye, &indicator) ;
        if(indicator != 0) {
            xfer.xfer_len = strlen(xfer.xfer_buf);
        }

    }while((err == EOK) && (indicator != 0)); 



   

    handle->close(handle);



    RETURN_VAL(0);

err_done:
    RETURN_VAL(ret);
}
