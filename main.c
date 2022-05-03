#include <stdio.h>
#include <stdlib.h>

#include "file_mgm.h"
#include "pump_debug.h"



int main(int argc, char *argv[])
{
#ifdef _DO_TEST_MEMCMP_x
    extern int test_memcmp32_s(void);
    test_memcmp32_s();

    extern int list_mgm_test(void);
    list_mgm_test();
#endif

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

    
    return 1;
}
