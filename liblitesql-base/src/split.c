/*  LiteSQL - C++ Object Persistence Framework
    Copyright (C) 2008  Tero Laitinen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "litesql.h"
#include <string.h>

int lsqlSplitNew(lsqlSplit* s) {
    memset(s, 0, sizeof(lsqlSplit));
    return 0;
}

void lsqlSplitDelete(lsqlSplit* s) {
    size_t i;

    for (i = 0; i < s->used; i++) 
        lsqlStringDelete(&s->parts[i]);


    lsqlFree(s->parts);
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
    s->used = size;
    return 0;
        
}
static size_t mapIndex(const lsqlSplit* s, int i) {
    if (i < 0) 
        return s->used + i;
    return i;
}

int lsqlSplitCopy(lsqlSplit* s, int i, const char* src) {
    size_t idx = mapIndex(s, i);

    if (idx >= s->used)
        return LSQL_ERROR;

    return lsqlStringCopy(&s->parts[idx], src);
}

int lsqlSplitCopy2(lsqlSplit* s, int i, const lsqlString* src) {
    size_t idx = mapIndex(s, i);

    if (idx >= s->used)
        return LSQL_ERROR;

    return lsqlStringCopy2(&s->parts[idx], src);
}
static int join(const lsqlSplit* s, lsqlString* dst, const char* delimPtr, 
                size_t delimSize) {
    size_t size = (s->used - 1) * delimSize;
    size_t i;
    char* ptr;

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
int lsqlSplitJoin(const lsqlSplit* s,
                  lsqlString* dst,
                  const char* delim) {
    return join(s, dst, delim, strlen(delim));
}
int lsqlSplitJoin2(const lsqlSplit* s,
                   lsqlString* dst,
                   const lsqlString* delim) {
    return join(s, dst, lsqlStringPtr(delim), lsqlStringSize(delim));
}

int lsqlSplitSlice(lsqlSplit* dst, const lsqlSplit* src,
                    int start, int end) {
    size_t i1 = mapIndex(src, start);
    size_t i2 = mapIndex(src, end);
    size_t elems, i;
    int ret;
    if (i1 >= src->used || i1 >= i2) 
        return lsqlSplitResize(dst, 0);

    if (i2 > src->used) 
        i2 = src->used;

    elems = i2 - i1;
    ret = lsqlSplitResize(dst, elems);

    if (ret)
        return ret;

    for (i = 0; i < elems; i++) {
        ret = lsqlSplitCopy2(dst, i, &src->parts[i1 + i]);
        if (ret)
            return ret;
    }

    return 0;
}

static int addOne(lsqlSplit* dst) {
    int ret;

    if (dst->used == dst->partsSize) {
        ret = lsqlSplitReserve(dst, 1 + (dst->partsSize * 2));
        if (ret)
            return ret;
    }
    ret = lsqlSplitResize(dst, dst->used + 1);
    if (ret)
        return ret;

    return 0;
}
int lsqlSplitAdd(lsqlSplit* dst, const char* src) {
    int ret = addOne(dst);
    if (ret)
        return ret;

    return lsqlSplitCopy(dst, dst->used - 1, src);

}
int lsqlSplitAdd2(lsqlSplit* dst, const lsqlString* src) {
    int ret = addOne(dst);
    if (ret)
        return ret;

    return lsqlSplitCopy2(dst, dst->used - 1, src);

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


