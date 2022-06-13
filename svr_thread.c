#include "pump_socket_common.h"
#include "pump_msg_macro.h"
#include "thread_mgm.h"

#include "curl_usage_api.h"

#define CONNECT_IP_ADDR     "127.0.0.1"
#define CONNECT_IP_PORT     12345



static const int XFER_START_PREFIX =           0x53855073;
static const int XFER_END_PREFIX =             0x83558049;


struct www_capture_info_t {
    char *www_path;
    char *www_url;
    int www_port;
    thread_mgm_t *thread;
};


struct svr_handle_t {
    socket_class_t *sck;
    fd_set read_fd_set;
    client_addr_t client;
    thread_mgm_t *th;
};


struct svr_transmit_t {
    socket_class_t *sck;
    thread_mgm_t *th;
    void *priv;
};

void *create_scr_socket(void)
{
    struct svr_handle_t *h = NULL;

    h = calloc(1, sizeof *h);
    h->sck = init_socket(AF_INET, SOCK_STREAM);
    
    // bind the socke
    h->sck->bind(h->sck, NULL, CONNECT_IP_PORT);

    // listen for incoming connections
    h->sck->listen(h->sck,5);


    return (void *)h;

}

static void parse_file_name(void *payload, int len)
{ 
    uint8_t *p = (uint8_t *)payload;
    uint32_t fstr_len = 0UL;

    p +=4; // skip the header
    len -= 4;

    while((fstr_len = strlen(p)) != 0) {
        debug_printf("File Name : %s (len:%d)", p, fstr_len);
        len -= (fstr_len+1);
        p += (fstr_len+1);
    }
    return;
}

static void* download_capture_image(void *priv)
{
#define PERSON_IDENTIFIER       "person"
    struct www_capture_info_t *p = (struct www_catpure_info_t *)priv;
    char curl_path[255] = {0, };
    char record_full_path[255] = {0, };
    char *record_path = NULL;
    THREAD_DETACH(p->thread);
    MUTEX_LOCK(p->thread);


    if(priv == NULL || p->www_url == NULL || p->www_path == NULL) {
        err_printf("Invalid (%p)", priv);
        return NULL;
    };

    COND_WAIT(p->thread);

    sprintf(curl_path, "http:\/\/%s\/%s", p->www_url, strstr(p->www_path, (const char *)"detected"));
    record_path = strstr((const char *)curl_path, (const char *)PERSON_IDENTIFIER);
    sprintf(record_full_path, "\.\/client_detected\/%s", record_path);
    info_printf("download path: %s \n download full path: %s ", curl_path, record_full_path); if(!curl_getinfo((const char *)curl_path)) { curl_url2file((const char *)curl_path, (const char *)record_full_path);
    }
    else {
        err_printf("%s file not existx on server", curl_path);
    }

    MUTEX_UNLOCK(p->thread);
    return priv;
}

void *svr_probe(void *priv)
{
#define CLIENT_IP_ADDRESS   "192.168.0.22"

    int ret = 0;
    int connections[5] = {-1, -1, -1, -1, -1};
    int i = 0;
    int fd_max = 0;
    uint8_t buf[64];
    uint8_t *_buf;

    int is_start = 0;
    int is_finish = 0;
    int recv_cnt = 1;
    client_addr_t client_addr; struct svr_handle_t *h = NULL; thread_mgm_t *p = (thread_mgm_t *)priv; socket_class_t *newsck = NULL;
    struct timeval timeout = {
        .tv_sec = 5,
        .tv_usec=5000
    };

    struct www_capture_info_t capture;

    fd_set reads;
    fd_set cpyreads;


    debug_printf("Thread ID : %ld, %ld", p->tid[0], pthread_self());
    //THREAD_DETACH(pthread_self());

    capture.thread = THREAD_MGM_INIT();

    capture.thread->start[0] = download_capture_image;
    capture.www_url = (char *)CLIENT_IP_ADDRESS;
    //sleep(10);

    //for(;;) {}

#if  1
    h = (struct svr_handle_t *)create_scr_socket();

    //connections[0] = h->sck->_sockfd;


    FD_ZERO(&reads);
    FD_SET(h->sck->_sockfd, &reads);
    fd_max = h->sck->_sockfd;

    //debug_printf("sockfd : %d", h->sck->_sockfd);
    //



    for(;;) {
        cpyreads = reads;
#if 0
        FD_ZERO(&h->read_fd_set);
        for( i = 0 ; i < 5 ; i ++ ) {
            if(connections[i] >= 0) {
                FD_SET(connections, &h->read_fd_set);
            }
        }
#endif
        ret = select(fd_max + 1, &cpyreads, NULL, NULL, &timeout);
        //ret = select(fd_max + 1, &cpyreads, NULL, NULL, NULL);
        if(ret == -1) {
            err_printf("select() failed - (%s)", SYS_ERROR_MSG());
            continue;
        }

        if(ret == 0) {
            timeout.tv_sec = 5;
            timeout.tv_usec = 5000;
            debug_printf("timeout - (%s)", SYS_ERROR_MSG());
#if 0
            if(recv_cnt > 1) {
                __BUF_HEX_PRINT(_buf, "recv packet", recv_cnt * 64);

                parse_file_name(_buf, recv_cnt * 64);
                MEM_RELEASE(_buf);
                is_start = is_finish = 0L;
                recv_cnt = 1;
            }
#endif
            continue;
        }

        for ( i = 0; i < fd_max+1; i++ ) {
            if(FD_ISSET(i, &cpyreads)) {
                if( i == h->sck->_sockfd ) {
                    newsck = h->sck->accept(h->sck, &client_addr);
                    debug_printf("accept sockfd : from %d to %d", h->sck->_sockfd,newsck->_sockfd);
                    FD_SET(newsck->_sockfd, &reads);
                    if(fd_max < newsck->_sockfd) {
                        fd_max = newsck->_sockfd;
                    }
                }
                else {
                    if(i != newsck->_sockfd) {
                        continue;
                    }
                    
                    ret = newsck->recv(newsck, buf, 64);
                    if(ret <= 0) {  //if you want to finish
                        debug_printf("socket transaction finish for new socket fd %d !!!", newsck->_sockfd);
                        FD_CLR(i, &reads);
                        socket_close(newsck);
                        usleep(5000);

                        FD_ZERO(&reads);
                        FD_SET(h->sck->_sockfd, &reads);
                    }
                    else {

                        //__BUF_HEX_PRINT(buf, "arrived packet",  64);
                        if(is_start) {
                            recv_cnt++;
                        }
                        //__BUF_HEX_PRINT(buf, "recv", 64);
                        if(!is_start) {
                            if(memcmp((void *)&buf[0], &XFER_START_PREFIX, 4) == 0) {
                                debug_printf("pakcet sync start");
                                MUTEX_LOCK(capture.thread);

                                if(THREAD_CREATE(&capture.thread->tid[0], NULL, capture.thread->start[0], (void *)&capture) != 0) {
                                    err_printf("%s", SYS_ERROR_MSG());
                                }

                                _buf = calloc(recv_cnt * 64, sizeof(char));
                                is_start = 1;
                                memcpy_s(_buf, 64, buf, 64);
                                //__BUF_HEX_PRINT(_buf, "*** recv ****", 64);
                            }

                            if(memcmp((void *)&buf[64-4], &XFER_END_PREFIX, 4) == 0) {
                                capture.www_path = _buf + 4;
                                COND_SIGNAL(capture.thread);
                                MUTEX_UNLOCK(capture.thread);
                                debug_printf(" one pakcet sync finish");
                                is_finish = 1;
                                //TODO : FILE DOWNLOAD and LIST ADD the buffer
                                __BUF_HEX_PRINT(_buf, "*** Payload ****", 64);
                                parse_file_name(_buf, recv_cnt * 64);
                                MEM_RELEASE(_buf);
                                is_start = is_finish = 0L;
                                recv_cnt = 1;
                            }
                        }

                        if(is_start && !is_finish) {
                            //debug_printf("recv_cnt : %d", recv_cnt);
                            if(recv_cnt >= 2) {
                                _buf = realloc(_buf, 64 * recv_cnt);
                                memcpy_s(&_buf[(recv_cnt - 1) * 64], 64, buf, 64);
                                //__BUF_HEX_PRINT(&_buf[(recv_cnt -1) * 64], "--- recv ---", 64);

                                if(memcmp((void *)&buf[64-4], &XFER_END_PREFIX, 4) == 0) {
                                    COND_SIGNAL(capture.thread);
                                    MUTEX_UNLOCK(capture.thread);
                                    debug_printf("pakcet sync finish");
                                    //is_finish = 1;
                                    //TODO : FILE DOWNLOAD and LIST ADD the buffer
                                    parse_file_name(_buf, recv_cnt * 64);
                                    MEM_RELEASE(_buf);
                                    is_start = is_finish = 0L;
                                    recv_cnt = 1;
                                }
                            }
                        }
                    }
                    
                    //_BUF
                }
            }
        }
#if 0
        if(ret >= 0) {
            debug_printf("select returned with %d", ret);
            if(FD_ISSET(h->sck->_sockfd, &h->read_fd_set)) {
                newsck = h->sck->accept(h->sck, &h->client);

                if(newsck->_sockfd >= 0) {
                    for( i = 0 ; i < 5 ; i ++ ) {
                        if(connections[i] < 0) {
                             connections[i]  = newsck->_sockfd; }
                    }
                }
                else {
                    err_printf("accept failed (%s)", SYS_ERROR_MSG());
                }

                ret--;
                if(!ret) contine;
            }

            for ( i = 0; i < 5; i++ ) {
                if( (connections[i] > 0)  &&
                        (FD_ISSET(connections[i], &h->read_fd_set))) {
                    debug_printf("Returned fd is : %d [Index : %d]", connections[i],i);
                    //TODO : Recv 
                    ret = recv();
                }
            }

        } 
#endif
    }
#endif 

    pthread_exit(NULL);




    return NULL;
}

