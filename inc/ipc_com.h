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
    mq_attr      attr;
    void         *priv;
    FUNC(int, send, (void *));
    FUNC(int, recv, (void *));
    FUNC(int, open, (void *));
    FUNC(void, close, (void *));

}ipc_com_t;


typedef struct _t_ipc_xfer {
    mqd_t       mq;
    mq_attr     attr;
    void        *xfer_buf;
    int         *xfer_len;
}ipc_xfer_t;



