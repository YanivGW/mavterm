#include <stdio.h>
#include <stdlib.h>

#define proj_assert(expr)                                           \
    do                                                              \
    {                                                               \
        if (!(expr))                                                \
        {                                                           \
            printf("Assertion failed - %s:%d", __FILE__, __LINE__); \
            exit(-1);                                               \
        }                                                           \
    } while (0)
