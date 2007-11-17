#include "litesql.h"
#include <string.h>

int lsqlSplitNew(lsqlSplit* s) {
    memset(s, 0, sizeof(lsqlSplit));
    return 0;
}

void lsqlSplitDelete(lsqlSplit* s) {
    size_t i;
    for (i = 0; i < s->partsSize; i++)
        lsqlStringDelete(&s->parts[i]);

    memset(s, 0, sizeof(lsqlSplit));    
}

int lsqlSplitReserve(lsqlSplit* s, size_t size) {
    s->parts = (lsqlString*) lsqlRealloc(s->parts, 
                                         sizeof(lsqlString) * size);
    if (!s->parts) {
        s->partsSize = 0;
        return LSQL_MEMORY;
    }
    s->partsSize = size;

    return 0;
}

int lsqlSplitResize(lsqlSplit* s, size_t size) {
    size_t i;

    if (s->partsSize < size) {

        int ret = lsqlSplitReserve(s, size);

        if (ret)
            return ret;

    }

    if (size < s->used) {
        for (i = size + 1; i < s->used; i++)
            lsqlStringDelete(&s->parts[i]);
        s->used = size;
    } else 
        for (i = s->used; i < size; i++) {
            int ret = lsqlStringNew(&s->parts[i]);  
            if (ret)
                return ret;
        }
    return 0;
        
}
static size_t mapIndex(const lsqlSplit* s, int i) {
    if (i < 0) 
        return s->used + i;
    return i;
}

int lsqlSplitCopy(lsqlSplit* s, int i, const lsqlString* src) {
    size_t idx = mapIndex(s, i);

    if (idx >= s->used)
        return LSQL_ERROR;

    return lsqlStringCopy2(&s->parts[idx], src);
}

int lsqlSplitJoin(const lsqlSplit* s,
                   lsqlString* dst,
                   const lsqlString* delim) {
    size_t delimSize = lsqlStringSize(delim);
    size_t size = (s->used - 1) * delimSize;
    size_t i;
    char* ptr;
    const char* delimPtr = lsqlStringPtr(delim);

    for (i = 0; i < s->used; i++)
        size += lsqlStringSize(&s->parts[i]);
    
    lsqlStringResize(dst, size);

    ptr = (char*) lsqlStringPtr(dst);

    for (i = 0; i < s->used; i++) {
        size_t partSize;
        if (i) {
            memcpy(ptr, delimPtr, delimSize);
            ptr += delimSize;
        }
        partSize = lsqlStringSize(&s->parts[i]);
        memcpy(ptr, lsqlStringPtr(&s->parts[i]), partSize);
        ptr += partSize;
    }
    return 0;
}

int lsqlSplitSlice(lsqlSplit* dst, const lsqlSplit* src,
                    int start, int end) {
    size_t i1 = mapIndex(src, start);
    size_t i2 = mapIndex(src, end);
    size_t elems, i;
    int ret;

    if (i1 >= src->used || i2 >= i1) 
        return lsqlSplitResize(dst, 0);

    if (i2 > src->used) 
        i2 = src->used;

    elems = i2 - i1;

    ret = lsqlSplitResize(dst, elems);

    if (ret)
        return ret;

    for (i = 0; i < elems; i++) {
        ret = lsqlSplitCopy(dst, i, &src->parts[i1 + i]);
        if (ret)
            return ret;
    }

    return 0;
}

int lsqlSplitAdd(lsqlSplit* dst, const lsqlString* src) {
    int ret;
    if (dst->used == dst->partsSize) {
        ret = lsqlSplitReserve(dst, 1 + (dst->partsSize * 2));
        if (ret)
            return ret;

    }

    dst->used++;
    return lsqlSplitCopy(dst, dst->used - 1, src);

}

int lsqlSplitSize(const lsqlSplit* s) {
    return s->used;
}

lsqlString* lsqlSplitElem(lsqlSplit* s, int i) {

    size_t idx = mapIndex(s, i);

    if (idx >= s->used)
        return NULL;
    return &s->parts[idx];
}


