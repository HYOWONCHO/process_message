

pthread_mgm_t *t;
pthread_mgm_t *g;

int thread_test_main(int argc, char **argv)
{
    void *ping(void *data);
    void *pong(void *data);

    int status;
    void (*thread_func[2])(void *);
    t = PTHREAD_MGM_INIT();
    g = PTHREAD_MGM_INIT();

    thread_func[0] = pong;
    thread_func[1] = ping;

    for(int i = 0L; i < 2;  i++) {
        MUTEX_LOCK(t);
        if(pthread_create(&t->pid[i], NULL, thread_func[i], (void *)NULL) < 0) {
            perror("error: ");
            exit(0);
        }

        printf("pid [%d] : %ld \n", i, t->pid[i]);

        COND_WAIT(t);
        MUTEX_UNLOCK(t);
    }

    for(int i = 0; i < 2; i ++) {
        pthread_join(t->pid[i], (void **)&status);
    }

    PTHREAD_MGM_DEINIT(t);
    PTHREAD_MGM_DEINIT(g);



    return 0;
}



void *ping(void *data)
{

    int i=0;
    int wait_sleep = 0;

    MUTEX_LOCK(t);

    COND_SIGNAL(t);

    MUTEX_UNLOCK(t);

    while(1) {

        MUTEX_LOCK(g);

        printf("%d : ping\n", i);

        COND_SIGNAL(g);

        COND_WAIT(g);

        MUTEX_UNLOCK(g);

        wait_sleep = random()%100;

        usleep(wait_sleep);

        i++;


    }


}

void *pong(void *data)
{

    int i = 0;

    MUTEX_LOCK(t);

    sleep(1);

    COND_SIGNAL(t);

    MUTEX_UNLOCK(t);

    while(1) {

        MUTEX_LOCK(g);
        printf("unlocking !!! \n ");
        COND_WAIT(g);
        printf("wake-up \n");

        printf("%d : pong\n", i);

        COND_SIGNAL(g);

        MUTEX_UNLOCK(g);

        i++;
    }
}

