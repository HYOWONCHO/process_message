#include <stdio.h>
#include <stdlib.h>

#include "file_mgm.h"
#include "pump_debug.h"
#include "pump_socket_common.h"
#include "ipc_com.h"
#include "file_mgm.h"

#include "safe_mem_lib.h"

#if defined(__USED_CURL__)
#include "curl_usage_api.h"
#endif


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



#if defined(_DO_TEST_IPCSVR_)
    int test_ipcsvr_main(int argc, char **argv);

    test_ipcsvr_main(argc, argv);
#endif

#if  0//defined(__USED_CURL__)
#define APACHE_SVR_ADDR="http://192.168.0.9/"
#define APACHE_SVR_DWNDIR="test/"
#define APACHE_SVR_DWNNAME="changelog.md"


    char *dwnurl = "http://192.168.0.9/test/changelog.md";
    char *dwnurlx = "http://192.168.0.9/test/xxgelog.md";
    //sprintf(dwnpath, "%s%s%s", )
#if 1
    if(!curl_getinfo((const char *)dwnurl)) {
        curl_url2file((const char *)dwnurl, (const char *)"./aaaa.md");
    }
    else {
        err_printf("%s file not exists on the server", dwnurl);
    }
#endif

    if(curl_getinfo((const char *)dwnurlx) < 0) {
        err_printf("%s file not exists on the server", dwnurlx);
    }
#endif

#if defined(__FILE_IO_TEST__)
#endif

#if defined(_APP_CLIENT_)
    char *buff;
    char *fname = "./readme_test.md";

    socket_class_t *s = NULL;
    record_file_t rec;
    file_io_t *fio;

    debug_printf("APPLICATION CLINET START!!!");
    if(file_mgm_init(&rec) != EOK) {
        file_mgm_deinit(&rec);
        exit(1);
    }

    fio = rec.fio;

    if(fio->open((void *)&rec, (const char *)fname) != EOK) {
        file_mgm_deinit(&rec);
        exit(1);
    }

    buff = calloc(fio->fsize, sizeof *buff);

    if(fio->read(fio->f_idfy, buff, fio->fsize) < 0) {
        file_mgm_deinit(&rec);
        free(buff);
        exit(1);
    }




    //__BUF_HEX_PRINT(buff, fname, fio->fsize);

    fio->close(fio->f_idfy);

    //file_mgm_deinit(&rec);
    //free(buff);
    

    debug_printf("APPLICATION CLINET !!!");


#if 1
    //Instantiate a socket object
    s = init_socket(AF_INET, SOCK_STREAM);

    //Connect to sever
    s->connect(s,(const char *)CONNECT_IP_ADDR, CONNECT_IP_PORT); 
    //while(1) {
    char *okbuf[64] = {0, };
#if 0
    memzero_s(buff, sizeof buff);
    n_print(">> ");
    // wait the user input
    if(fgets(buff, sizeof(buff), stdin) == NULL) {
        perror("fgets error");
        break;
    }
#endif

    __BUF_HEX_PRINT(buff, "Sending", fio->fsize);


    s->send(s, buff, fio->fsize);

    //s->recv(s,buff, sizeof(buff));

    //n_print("Received : %s\n", buff);

    //if(buff[0] == 'o' && buff[1] == 'k') {
    //    s->send(s, (const char *)"ok", 2);
    //}
    //}
    socket_close(s);
#endif
#endif


#if defined(_APP_SERVER_)
    char *fname = "./readme_test_svr.md";
    char buff[1093];
    int bytes;

    record_file_t rec;
    file_io_t *fio;

    debug_printf("APPLICATION SERVER !!!");
    socket_class_t *s = init_socket(AF_INET, SOCK_STREAM);


    s->bind(s, NULL, CONNECT_IP_PORT);
    s->listen(s, 5);

    client_addr_t client_addr;

    socket_class_t *c = s->accept(s, &client_addr);

    n_print("Client connection -> %s:%d \n", client_addr.ip_addr, client_addr.port);
    //client_addr.port = 12345;


    //while(1) {
    memzero_s((void *)buff, sizeof(buff));
    c->recv(c, buff, sizeof(buff));

    __BUF_HEX_PRINT(buff, "receive data", sizeof buff);

    if(file_mgm_init(&rec) != EOK) {
        file_mgm_deinit(&rec);
        exit(1);
    }

    fio = rec.fio;

    if(fio->open((void *)&rec, (const char *)fname) != EOK) {
        file_mgm_deinit(&rec);
        exit(1);
    }

    fio->write(fio->f_idfy, buff, sizeof(buff));

    fio->close(fio->f_idfy);


       // debug_printf("recv buffer : %s", buff);
       // if(strlen(buff) == 0) {
       //     break;
       // }

        // n_print("Got: %s \n", buff);
        //c->send(c, (const char *)"ok",2);
        //n_print("Sending : %s \n", buff);
    //}

    socket_close(c);
    socket_close(s);
#endif

    
    return 1;
}
