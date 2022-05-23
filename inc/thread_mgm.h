#pragma once


#include <pthread.h>

typedef struct pthread_mgm_t {
    pthread_t pid[5];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_condattr_t attr;
    int flag;
}pthread_mgm_t;


#define PTHREAD_MGM_INIT()                                           \
    ({                                                               \
        pthread_mgm_t *t;                                            \
        t = calloc(1, sizeof *t);                                    \
        memset((void *)t->pid, 0x0, sizeof t->pid);                  \
        t->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;       \
        t->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;          \
        pthread_condattr_init(&t->attr);                             \
        t;                                                           \
    })



#define PTHREAD_MGM_DEINIT(x)                                       \
    ({                                                              \
        pthread_mgm_t *_t = (pthread_mgm_t *)x;                     \
        pthread_mutex_destroy(&_t->mutex);                          \
        pthread_condattr_destroy(&_t->attr);                        \
        pthread_cond_destroy(&_t->cond);                            \
        free(x);                                                    \
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


