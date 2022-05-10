#include "ipc_com.h"

static void ipc_com_close(void *p);
static int ipc_com_open(void *p);
static int ipc_com_receive(void *p);
static int ipc_com_receive(void *p);
static int ipc_com_transmit(void *p);

int ipc_com_create(void **p, void *buf, int mode)
{
    int ret = -1;
    ipc_com_t *h = NULL;;
    //int oflag = 0L;

    if( (p==NULL) || (buf == NULL) ) {
        err_printf("Invalid argument");
        goto err_done;
    }

    *p = (void *)calloc(1, sizeof *h);
    if(*p == NULL) {
        err_printf("Failed the memory allocation");
        goto err_done;
    }

    h = *p;
    h->priv = buf;

    switch(mode) {
        case IPC_TYPE_SERVER:
            h->mq_attr.mq_flags = O_WRONLY | O_CREAT;
            break;
        case IPC_TYPE_CLIENT:
            h->mq_attr.mq_flags = O_WRONLY;
            break;
        default:
            err_printf("Invalid MODE argument");
            goto err_done;
    }


    h->send = ipc_com_transmit;
    h->recv = ipc_com_receive;
    h->open = ipc_com_open;
    h->close = ipc_com_close;
    

    ret = 0L;
    return ret;

err_done:
    if(h != NULL) {
        free(h);
    }

    return ret;
}

static void ipc_com_close(void *p)
{
    if(p->mq) {
        mq_close(p->mq);
    }

    free(p);
    p = NULL;

}

static int ipc_com_open(void *p)
{
    ipc_com_t *h = (ipc_com_t *)p;

    h->mq = mq_open(IPC_COM_PATH, h->mq_attr.mq_flags); 
    if(h->mq == -1) {
        err_printf("Failed message queue open (%s)", strerror(errno));
        goto err_done;
    }

    ((ipc_xfer_t *)h->priv)->mq = h->mq;
    ((ipc_xfer_t *)h->priv)->attr.mq_flags = h->attr.mq_flags;
    ((ipc_xfer_t *)h->priv)->attr.mq_maxmsg = h->attr.mq_maxmsg;
    ((ipc_xfer_t *)h->priv)->attr.mq_msgsize = h->attr.mq_msgsize;
    ((ipc_xfer_t *)h->priv)->attr.mq_curmsg = h->attr.mq_curmsg;

    
    return 0L;

err_done:
    return -1;

}

static int ipc_com_receive(void *p)
{
    int ret = -1;

err_done:
    return ret;

}

static int ipc_com_transmit(void *p)
{
    int ret = -1;
    ipc_xfer_t *xfer = ((ipc_com_t *)p)->priv;


    if( h == NULL ) {
        err_printf("Invalid argument");
        goto err_done;
    }

    ret = mq_send(xfer->mq, xfer->xfer_buf, xfer->xfer_len, IPC_PRIORITY_OF_MSG);
    if(ret == -1) {
        goto err_done;
    }

    ret = 0L;
    return ret;
err_done:
    return ret;
}
