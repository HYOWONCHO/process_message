#pragma once


#include <pthread.h>

#define THREAD_MAX_CNT          5

typedef void *(*pthread_start)(void *);

typedef struct pthread_mgm_t {
    int idx;
    pthread_t pid[THREAD_MAX_CNT];
    pthread_start start[THREAD_MAX_CNT]; 
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_condattr_t attr;
}pthread_mgm_t;


#define THREAD_MGM_INIT(func)                                        \
    ({                                                               \
        pthread_mgm_t *t;                                            \
        t = calloc(1, sizeof *t);                                    \
        memset((void *)t->pid, 0x0, sizeof t->pid);                  \
        t->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;       \
        t->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;          \
        pthread_condattr_init(&t->attr);                             \
        t;                                                           \
    })


#define THREAD_MGM_DEINIT(x)                                        \
    ({                                                              \
        pthread_mgm_t *_t = (pthread_mgm_t *)x;                     \
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
        pthread_detach((pthread_t *)tid);                           \
    })

#define MUTEX_LOCK(x)                                               \
    ({                                                              \
        pthread_mutex_lock(&(((pthread_mgm_t *)x)->mutex));         \
    })

#define MUTEX_UNLOCK(x)                                             \
    ({                                                              \
        pthread_mutex_unlock(&(((pthread_mgm_t *)x)->mutex));       \
    })

#define COND_SIGNAL(x)                                              \
    ({                                                              \
        pthread_cond_signal(&((pthread_mgm_t *)x)->cond);           \
    })

#define COND_SIGNAL_WITH_ATTR(x)                                    \
    ({                                                              \
        pthread_cond_signal(&((pthread_mgm_t *)x)->cond,            \
                &((pthread_mgm_t *)x)->attr);                       \
    })

#define COND_WAIT(x)                                                \
    ({                                                              \
        pthread_cond_wait(&((pthread_mgm_t *)x)->cond,              \
                        &((pthread_mgm_t *)x)->mutex);              \
    })


#define COND_BROADCAST(x)                                           \
    ({                                                              \
        pthread_cond_broadcast(&(pthread_mgm_t *x)->cond);          \
    })

