#pragma once 

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include "pump_msg_macro.h"


typedef enum _t_socket_type {
    INET_SOCKET_SERVER      = 1,
    INET_SOCKET_CLIENT,
    INET_SOCKET_UNKNOWN
}socket_type_t;


/**
 * client_addr_t - Structure to hold the information  for connected client
 */
typedef struct _t_clientaddr {
    const char *ip_addr;        /**< Connected client address */
    int port;                   /**< Connected port number */
}client_addr_t;

typedef struct socket_select_t {
    struct timeval tv;
    fd_set fds;
}socket_select_t;


typedef struct socket_class_t {
    /**
     * connect - connect to server
     *
     * @param socket_class_t*       IN pointer to the socket object 
     * @param const char *          IN address of the server to connect to
     * @param int                   IN Port of the remote server
     *
     * @return NONE
     */
    FUNC(void, connect, (struct socket_class_t*, const char *, int));

    FUNC(void, bind, (struct socket_class_t*, const char *, int));

    FUNC(void, listen,(struct socket_class_t*,int));

    FUNC(struct socket_class_t* ,accept,(struct socket_class_t*, client_addr_t*));

    FUNC(int, send, (struct socket_class_t*, const char *, int));

    FUNC(int, recv, (struct socket_class_t*, char *, int));

    FUNC(int, select, (struct socket_class_t*, int));


    int _sockfd;
    int _family;
    int _type;
    int _proto;
    struct addrinfo _addrinfo_hints;
    struct addrinfo*_addrinfo;
    socket_select_t fd_select;

    FUNC(void, _create, (struct socket_class_t *, int, const char *, int));

    FUNC(void, _exit_error, (struct socket_class_t *, const char *));

    FUNC(char *, _inet_ntop, (struct socket_class_t *, struct sockaddr *));


    pthread_mutex_t mutex;
    pthread_cond_t  cond;
}socket_class_t;



socket_class_t *init_socket(int family, int type);
void socket_close(socket_class_t *s);
