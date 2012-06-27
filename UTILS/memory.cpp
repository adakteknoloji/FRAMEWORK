#include <stdlib.h>
#include "adak_utils.h"

#define MEMORY_MARK   0x7FA4

/***************************************/
/*               MALLOC                */
/***************************************/

char * MALLOC (unsigned int request_size)
{
    char *         p;

    if (request_size EQ 0) {
        // MEMORY_ERROR (E_INVALID_REQ, request_size);
    }
    p = (char *) malloc(request_size+sizeof(int)*3);
    if (p EQ NULL) {
        // MEMORY_ERROR (E_CANNOT_ALLOC, request_size);
    }
    *((int *)p) = MEMORY_MARK;
    p += sizeof(int);
    *((int *)p) = request_size;
    p += sizeof(int);
    memset (p, 0, request_size);
    p += request_size;
    *((int *)p) = MEMORY_MARK;
    p -= request_size;
    return (p);
}

/***************************************/
/*               FREE                  */
/***************************************/

void FREE(void * q)
{
    char *         p = (char *) q;
    int            request_size;

    if (p EQ NULL) {
        // MEMORY_ERROR (E_CANNOT_FREE, -1);
    }
    p -=sizeof(int)*2;
    if (*((int *)p) NE MEMORY_MARK) {
        // MEMORY_ERROR (E_MEMORY_DESTROYED , -1);
    }
    p += sizeof(int);
    request_size = *((int *)p);
    p += request_size + sizeof(int);
    if (*((int *)p) NE MEMORY_MARK) {
        // MEMORY_ERROR (E_MEMORY_DESTROYED , -1);
    }
    p = (char *) q;
    p -= sizeof(int)*2;
    memset (p, 0, request_size + sizeof(int)*3);
    free(p);
}
