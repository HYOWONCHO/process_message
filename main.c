#include <stdio.h>
#include <stdlib.h>

#include "file_mgm.h"
#include "pump_debug.h"
#include "pump_socket_common.h"

#include "safe_mem_lib.h"


#define CONNECT_IP_ADDR     "127.0.0.1"
#define CONNECT_IP_PORT     12345


int main(int argc, char *argv[])
{
#ifdef _DO_TEST_MEMCMP_x
    extern int test_memcmp32_s(void);
    test_memcmp32_s();

    extern int list_mgm_test(void);
    list_mgm_test();
#endif

#if 0
    extern int record_stream_list(void **p, int size);
    record_file_t p;
    list_element_t *pos = NULL;
    file_mgm_init(&p);
    p.is_first = false;
    if(record_stream_list(&p,  1) > 0) {
        pos = list_head(p.list);
        for(int i = 0; i < list_get_size(p.list); i++) {
            info_printf("Data : %s", (char *)pos->data);
            pos = list_next(pos);
        }
    }

    list_destroy(&p);
#endif

#if defined(_APP_CLIENT_)
    char buff[100];

    socket_class_t *s = NULL;


    debug_printf("APPLICATION CLINET !!!");

    //Instantiate a socket object
    s = init_socket(AF_INET, SOCK_STREAM);

    //Connect to sever
    s->connect(s,(const char *)CONNECT_IP_ADDR, CONNECT_IP_PORT); 
    while(1) {
        memzero_s(buff, sizeof buff);
        n_print(">> ");

        // wait the user input
        if(fgets(buff, sizeof(buff), stdin) == NULL) {
            perror("fgets error");
            break;
        }

        s->send(s, buff);


        s->recv(s,buff, sizeof(buff));

        n_print("Received : %s\n", buff);
    }
    socket_close(s);
#endif


#if defined(_APP_SERVER_)
    char buff[100];
    int bytes;

    debug_printf("APPLICATION SERVER !!!");
    socket_class_t *s = init_socket(AF_INET, SOCK_STREAM);


    s->bind(s, NULL, 12345);
    s->listen(s, 5);

    client_addr_t client_addr;

    socket_class_t *c = s->accept(s, &client_addr);

    n_print("Client connection -> %s:%d \n", client_addr.ip_addr, client_addr.port);


    while(1) {
        memzero_s((void *)buff, sizeof(buff));
        c->recv(c, buff, sizeof(buff));


        if(strlen(buff) == 0) {
            break;
        }

        n_print("Got: %s \n", buff);
        c->send(c, buff);
        n_print("Sending : %s \n", buff);
    }

    socket_close(c);
    socket_close(s);
#endif

    
    return 1;
}
