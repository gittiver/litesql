#include "internal.h"
#include <string.h>
int lsqlAddToArray(void** array, size_t* size, size_t elemSize, void** elem) {

    (*size)++;
    *array = lsqlRealloc(*array, elemSize * (*size));
   
    if (!*array) {
        *elem = NULL;
        return LSQL_MEMORY;
    }

    *elem = &((char*)*array)[((*size)-1)*elemSize];    
    memset(*elem, 0, elemSize);
    return 0;
}
