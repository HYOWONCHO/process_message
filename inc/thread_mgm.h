#pragma once


#include <pthread.h>
#include "list_manager.h"

#define THREAD_MAX_CNT          5

typedef void *(*pthread_start)(void *);

typedef struct thread_mgm_t {
    int idx;
    pthread_t tid[THREAD_MAX_CNT];
    pthread_start start[THREAD_MAX_CNT]; 
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_condattr_t attr;

    list_element_t *s;
}thread_mgm_t;


typedef struct ai2file_sync_t {
    thread_mgm_t ai2f;


}ai2file_sync_t;

typedef struct io2net_sync_t {
    thread_mgm_t io2net;
    list_element_t *flist;    // File element list


}io2net_sync_t;

#define THREAD_MGM_INIT()                                            \
    ({                                                               \
        thread_mgm_t *t;                                             \
        t = calloc(1, sizeof *t);                                    \
        memset((void *)t->tid, 0x0, sizeof t->tid);                  \
        t->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;       \
        t->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;          \
        pthread_condattr_init(&t->attr);                             \
        t;                                                           \
    })


#define THREAD_MGM_DEINIT(x)                                        \
    ({                                                              \
        thread_mgm_t *_t = (thread_mgm_t *)x;                       \
        pthread_mutex_destroy(&_t->mutex);                          \
        pthread_condattr_destroy(&_t->attr);                        \
        pthread_cond_destroy(&_t->cond);                            \
        free(x);                                                    \
    })

#define THREAD_CREATE(tid,attr,func,arg)                            \
    ({                                                              \
        pthread_create((pthread_t *)tid, (pthread_attr_t *)attr,    \
                (pthread_start)func, (void *)arg);                  \
    })

/**
 * THREAD_JOIN - Wait for a thread to end
 *
 * @param[in] tid - Thread handle
 * @param[out] retval - If retval is not NULL, Thread exit status of the target thread
 *
 * @return On success, return 0, otherwise, returns an error number
 */
#define THREAD_JOIN(tid,retval)                                     \
    ({                                                              \
        pthread_join((pthread_t)tid, (void **)retval);              \
    })


/**
 * THREAD_DETACH - Detach a thread
 *
 * @param[in] tid - Thread ID
 *
 * @return On success, return 0, othrewise, return -1
 */
#define THREAD_DETACH(tid)                                          \
    ({                                                              \
        pthread_detach((pthread_t)tid);                             \
    })

#define MUTEX_LOCK(x)                                               \
    ({                                                              \
        pthread_mutex_lock(&(((thread_mgm_t *)x)->mutex));         \
    })

#define MUTEX_UNLOCK(x)                                             \
    ({                                                              \
        pthread_mutex_unlock(&(((thread_mgm_t *)x)->mutex));       \
    })

#define COND_SIGNAL(x)                                              \
    ({                                                              \
        pthread_cond_signal(&((thread_mgm_t *)x)->cond);           \
    })

#define COND_SIGNAL_WITH_ATTR(x)                                    \
    ({                                                              \
        pthread_cond_signal(&((thread_mgm_t *)x)->cond,            \
                &((thread_mgm_t *)x)->attr);                       \
    })

#define COND_WAIT(x)                                                \
    ({                                                              \
        pthread_cond_wait(&((thread_mgm_t *)x)->cond,              \
                        &((thread_mgm_t *)x)->mutex);              \
    })


#define COND_BROADCAST(x)                                           \
    ({                                                              \
        pthread_cond_broadcast(&(thread_mgm_t *x)->cond);          \
    })

