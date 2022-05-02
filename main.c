#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
#ifdef _DO_TEST_MEMCMP_
    extern int test_memcmp32_s(void);
    test_memcmp32_s();

    extern int list_mgm_test(void);
    list_mgm_test();
#endif

    extern int record_stream_list(void **p, int size);
    char *p = NULL;
    record_stream_list(&p,  1);
    
    return 1;
}
