#include <stdio.h>
#include <stdlib.h>

#include "file_mgm.h"
#include "pump_msg_macro.h"
#include "pump_socket_common.h"
#include "ipc_com.h"
#include "file_mgm.h"
#include "thread_mgm.h"
#include "safe_mem_lib.h"



#if defined(__USED_CURL__)
#include "curl_usage_api.h"
#endif







int main(int argc, char *argv[])
{

    void *svr_probe(void *priv);


    thread_mgm_t *transmit = NULL;

    transmit = THREAD_MGM_INIT();
    if(transmit == NULL) {
        err_printf("%s", SYS_ERROR_MSG());
        return -1;
    }
    
    transmit->start[0] = svr_probe;
    if(THREAD_CREATE(&transmit->tid[0], NULL, transmit->start[0], (void *)transmit) < 0) {
        err_printf("%s", SYS_ERROR_MSG());
        return -1;
    }

    return 0;
}
