#include "litesql.h"
#include <string.h>
int lsqlStringNew(lsqlString* s) {
    memset(s, 0, sizeof(lsqlString));
    return 0;
}

int lsqlStringCopy(lsqlString* dst, const char* src) {
    dst->used = dst->size = strlen(src);
    dst->data = lsqlRealloc(dst->data, dst->size);

    if (dst->data == NULL)
        return LSQL_MEMORY;

    memcpy(dst->data, src, dst->size);
    return 0;
}

int lsqlStringCat(lsqlString* dst, lsqlString* src) {
    /* TODO: strcat */
    return 0;
}

void lsqlStringDelete(lsqlString* s) {
    lsqlFree(s->data);
    memset(s, 0, sizeof(lsqlString));
}


