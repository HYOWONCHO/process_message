#include "pump_debug.h"
#include "safe_mem_lib.h"


#define LEN (128)


int test_memcmp32_s(void) 
{
    errno_t rc;
    uint32_t len;
    int32_t ind;
    int32_t std_ind;
    uint32_t i;

    uint32_t mem1[LEN] = {0, };
    uint32_t mem2[LEN] = {1, };

    debug_printf("%s function testing !!! \n", __FUNCTION__);

    rc = memcmp32_s(NULL,LEN,mem2, LEN, &ind);
    if(rc != ESNULLP) {
        debug_printf("%s %u Ind=%d Error rc=%u \n", __FUNCTION__, __LINE__, ind, rc);
    }


    for (i=0; i<LEN; i++) { mem1[i] = i; }

    for (i=0; i<LEN; i++) { mem2[i] = i; }



    len = 1;

    rc = memcmp32_s(mem1, len, mem2, len, &ind);

    if (rc != EOK) {

        debug_printf("%s %u  Ind=%d  Error rc=%u \n",

                     __FUNCTION__, __LINE__, ind, rc );

    }



    if (ind != 0) {

        debug_printf("%s %u  Ind=%d  rc=%u \n",

                     __FUNCTION__, __LINE__,  ind, rc );

    }



    std_ind = memcmp(mem1, mem2, len*4);

    if (ind != 0) {

        debug_printf("%s %u  Ind=%d  rc=%u \n",

                     __FUNCTION__, __LINE__,  ind, rc );

    }



    return (0);

}






