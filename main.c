#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
#ifdef _DO_TEST_MEMCMP_
    extern int test_memcmp32_s(void);
    test_memcmp32_s();
#endif
    return 1;
}
