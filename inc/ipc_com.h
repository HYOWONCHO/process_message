#pragma once

#include "pump_msg_macro.h"

#include <mqueue.h>
#include <pthread.h>
#include <unistd.h>

/**< Message queue path */
#define IPC_COM_PATH            "/ipccomqueue"

#define IPC_COM_N_MESSAGES      2000000

#define IPC_TYPE_SERVER         0x00
#define IPC_TYPE_CLIENT         0x01

#define IPC_PRIORITY_OF_MSG     0x01

typedef struct _t_ipc_com {
    mqd_t        mq;
    struct mq_attr      attr;
    void         *priv;
    FUNC(int, send, (void *));
    
    /**
     * recv - receive a message from a message queue
     *
     * @param priv
     *
     * @return  On success, return the number of byutes in the received message. 
     *          On error, -1 is returned with
     *
     */ 
    FUNC(int, recv, (void *));

    /**
     * open - create a new POSIX message queue or opens an existing queue
     *
     * @param priv      
     * @return  On success, returns a message queue descriptor. \n
     *          On erroe, return -1, with errno set to indicate the error
     */
    FUNC(int, open, (void *));
    FUNC(void, close, (void *));

}ipc_com_t;


typedef struct _t_ipc_xfer {
    mqd_t       mq;
    struct mq_attr     attr;
    void        *xfer_buf;
    int         *xfer_len;
}ipc_xfer_t;



