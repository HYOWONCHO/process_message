#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#include "pump_socket_common.h"
#include "pump_msg_macro.h"
#include "pump_debug.h"

#include "safe_mem_lib.h"
#include "snprintf_s.h"



static void _connect_(socket_class_t *s, const char *ip_addr, int port)
{
    s->_create(s,INET_SOCKET_CLIENT,ip_addr, port);
    if(connect(s->_sockfd, s->_addrinfo->ai_addr,s->_addrinfo->ai_addrlen) == -1) {
        s->_exit_error(s, "Error connecting to the remote server");
    }

    freeaddrinfo(s->_addrinfo);
}



static void _bind_(socket_class_t *s, const char *ip_addr, int port)
{
    s->_create(s, INET_SOCKET_SERVER, ip_addr, port);
    if(bind(s->_sockfd, s->_addrinfo->ai_addr, s->_addrinfo->ai_addrlen) == -1) {
        s->_exit_error(s, "Error binding the socket");

    }

    freeaddrinfo(s->_addrinfo);
}


static void _listen_(socket_class_t *s, int max_queue)
{
    if(listen(s->_sockfd, max_queue) == -1) {
        s->_exit_error(s, "Error listening for connections");
    }
}

static socket_class_t *_accept_(socket_class_t *s, client_addr_t *c)
{
    struct sockaddr_in client_addr;
    unsigned addr_size = sizeof(client_addr);
    int client_fd;

    client_fd = accept(s->_sockfd, (struct sockaddr *)&client_addr, &addr_size);
    if(client_fd < 0) {
        s->_exit_error(s, "Ereror accepting a remote connection");

    }

    if(c) {
        //return client struct with client ip and port
        c->ip_addr = s->_inet_ntop(s, (struct sockaddr *)&client_addr);
        c->port = client_addr.sin_port;
    }

    //return a socket object initialized with client socke4t
    socket_class_t *sock = init_socket(s->_family, s->_type);
    sock->_sockfd = client_fd; 
    return sock;
}


static int _send_(socket_class_t *s, const char *str, int max_len)
{
    debug_printf("fd : %d , len : %d", s->_sockfd, max_len);
    return send(s->_sockfd, str, max_len, 0);
}


static int _recv_(socket_class_t *s, char *buff, int max_len)
{
    int bytes_recv;
    char tmp[max_len];

    // clear buffer
    memzero_s(tmp, max_len);
    memzero_s(buff, max_len);

    bytes_recv = recv(s->_sockfd, tmp, max_len, 0);
    //snprintf(buff, max_len, "%s", tmp);
    memcpy_s((void *)buff, max_len, (void *)tmp, max_len);

    return bytes_recv;
}


//Create a socket
static void _create_(socket_class_t *s, int type, const char *ip_addr, int port)
{
    char sport[6];
    int ret = 0;

    if(snprintf_s_i((char *)sport, sizeof sport, "%d", port) <= 0) {
        s->_exit_error(s, "Error converting port number to string");
    }

    //debug_printf("sport : %s", sport);
    if(type == INET_SOCKET_SERVER) {
        s->_addrinfo_hints.ai_flags = AI_PASSIVE;
    }

    // get the addrinfo structure to use to cerate the socket
    if(getaddrinfo(ip_addr, sport, &(s->_addrinfo_hints), &(s->_addrinfo)) != 0)  {
        s->_exit_error(s, "Error getting address info");
    }

    // create a socket
    //
     if((s->_sockfd = socket(s->_addrinfo->ai_family,
                     s->_addrinfo->ai_socktype, s->_addrinfo->ai_protocol)) == -1) {
        s->_exit_error(s, "Error creaing a socket");
     }

}

static int _read_select_(struct socket_class_t *s, int sec)
{
#if 0
    int ret = EOK;

    socket_select_t *handle = NULL;

    handle = (socket_select_t *)ptr_member_of_container(s, socket_class_t, fd_select);

    FD_ZERO(&handle->fds);
    FD_SET(s->_sockfd + 1, &handle->fds);

    handle->tv.tv_sec = sec;
    handle->tv.tv_usec = 0;


    if(sec == 0) {
        ret = select(s->_sockfd + 1, &handle->fds, NULL, NULL, NULL);
    }
    else
        ret = select(s->_sockfd + 1, &handle->fds, NULL, NULL, &handle->tv);
    }

    switch(ret) {
        case -1:
            err_printf("select failed (%s)", SYS_ERROR_MSG());
            RETURN_VAL(ret);
        case 0:
            debug_printf("NO data within %d seconds.", handle->tv.tv_sec);
            RETURN_VAL(EFAIL);
        default:
            break;
    }

    if(!FD_ISSET(s->_sockfd, &handle->fds)) {
        info_printf("%s", SYS_ERROR_MSG());
        return EFAIL;
    }


    debug_printf("Data is available now.");
#endif
    return EFAIL;
}

static void _exit_error_(socket_class_t *s, const char *msg)
{
    socket_close(s);

    if(errno != 0) {
        err_printf("(%s) %s", strerror(errno), msg);
        //exit(1);
    }

    err_printf("%s", msg);

}


static char *_inet_ntop_(socket_class_t *s, struct sockaddr *sa)
{
    static char str[INET6_ADDRSTRLEN];
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),str, INET_ADDRSTRLEN);
            break;
        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),str, INET6_ADDRSTRLEN);
            break;
        default:
            strncpy(str, "Unknown", INET_ADDRSTRLEN);
            return NULL;
    }

    return str;
}




socket_class_t *init_socket(int family, int type)
{
    socket_class_t *s;

    if((s = (socket_class_t *)calloc(1, sizeof *s)) == NULL) {
        err_printf("socket allocation failed (%s)", strerror(errno));
        return NULL;
    }

    // Mapping the function pointer
    s->connect          = _connect_;
    s->bind             = _bind_;
    s->listen           = _listen_;
    s->accept           = _accept_;
    s->send             = _send_;
    s->recv             = _recv_;
    s->_create          = _create_;
    s->_inet_ntop        = _inet_ntop_;
    s->_exit_error      = _exit_error_;


    s->_sockfd = -1;
    s->_family = family;
    s->_type = type;
    s->_proto = 0; 
    

    memzero_s(&(s->_addrinfo_hints), sizeof(s->_addrinfo_hints));

    s->_addrinfo_hints.ai_family = family;
    s->_addrinfo_hints.ai_socktype = type;


    //pthread_mutex_init(&s->lock)
    s->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    s->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    return s;
}


void socket_close(socket_class_t *s)
{
    if(s) {
        close(s->_sockfd);

        MEM_RELEASE(s);
    }
}


