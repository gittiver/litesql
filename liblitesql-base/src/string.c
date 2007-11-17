#include "litesql.h"
#include <string.h>

/** decodes the variable length size information from the beginning of the
 * string, returns the length of the string data in bytes */
static int readSize(const lsqlString* s) {
    size_t size = 0;

    unsigned char* p = s->data;
    unsigned int b = 0;
    while (1) {
        size |= ((*p) & 0x7f) << b;
        if (((*p) & 0x80) == 0)
            break;
        p++; 
        b += 7;
    }
   

    return size;
}

/* encodes the size information in the beginning of the string data */
static void writeSize(lsqlString* s, size_t size) {
    
    unsigned char c = 0;
    unsigned char* p = s->data;

    if (!size) {
        *p = 0;
        return;
    }

    for (; size; p++) {
        c = size & 0x7f;

        /** each byte contains 7 bits of size information and an additional
         * bit to indicate that the next byte contains the next 7 bits
         * of the size information */
        size >>= 7;
        if (size)
            c |= 0x80;
        *p = c;                
    }
}

/* returns the size of the length information field in bytes 
 * e.g. how many bytes is required to encode the given size number */
static int stringSize(size_t dataSize) {

    if (dataSize < 1 << 7)
        return 1 + dataSize;
    else if (dataSize < 1 << 14)
        return 2 + dataSize;
    else if (dataSize < 1 << 21)
        return 3 + dataSize;
    else if (dataSize < 1 << 28)
        return 4 + dataSize;
    else  /* 35 bits should be big enough to encode the length of a string */
        return 5 + dataSize;
}

static unsigned char* dataStart(const lsqlString* s) {
    unsigned char* p = s->data;

    /* the last byte of the size information is guaranteed to have its
     * most significant bit zero */
    while (*p & 0x80) 
        p++;

    return ++p;
}

int lsqlStringNew(lsqlString* s) {
    memset(s, 0, sizeof(lsqlString));
    return 0;
}

int lsqlStringResize(lsqlString* dst, size_t size) {
    size_t total = stringSize(size);
    size_t oldSize;
    unsigned char* origStart, *newStart;

    if (!dst->data) {
        dst->data = lsqlMalloc(total + 1);
        if (!dst->data)
            return LSQL_MEMORY;
        writeSize(dst, size);
        dst->data[total] = '\0';

    }

    origStart = dataStart(dst);
    oldSize = readSize(dst);
    dst->data = lsqlRealloc(dst->data, total + 1);
    dst->data[total] = '\0';

    if (!dst->data) 
        return LSQL_MEMORY;
    
    newStart = dataStart(dst);

    if (origStart != newStart) {
        if (oldSize > size)
            oldSize = size;
        memmove(newStart, origStart, oldSize);
    }
    writeSize(dst, size);

    return 0;
}


int lsqlStringCopy(lsqlString* dst, const char* src) {

    size_t size = strlen(src);
    size_t total = stringSize(size);


    dst->data = lsqlRealloc(dst->data, total + 1);

    if (dst->data == NULL)
        return LSQL_MEMORY;
    
    writeSize(dst, size);
    memcpy(dataStart(dst), src, size);

    /* to ensure compatibility with C string functions */
    dst->data[total] = '\0';

    return 0;
}

int lsqlStringCopy2(lsqlString* dst, const lsqlString* src) {
    size_t size, total;

    if (!src->data) {
        dst->data = NULL;
        return 0;
    }
    size = readSize(src);
    total = stringSize(size);

    dst->data = lsqlRealloc(dst->data, total + 1);

    if (dst->data == NULL)
        return LSQL_MEMORY;

    memcpy(dst->data, src->data, total + 1);

    return 0;
}

int lsqlStringCat(lsqlString* dst, const lsqlString* src) {
    /* TODO: strcat */
    return 0;
}

size_t lsqlStringSize(const lsqlString* s) {

    if (!s->data)
        return 0;

    return readSize(s);
}

void lsqlStringDelete(lsqlString* s) {

    lsqlFree(s->data);
    memset(s, 0, sizeof(lsqlString));
}

const char* lsqlStringPtr(const lsqlString* s) {
    if (!s->data)
        return "NULL";
    return (const char*) dataStart(s); 
}

int lsqlStringCmp(const lsqlString* s, const char* s2) {
    if (!s->data)
        return -1;
    return strcmp(lsqlStringPtr(s), s2);
}

int lsqlStringCmp2(const lsqlString* s1, const lsqlString* s2) {
    if (s1->data == s2->data)
        return 0;
    if (s2->data == NULL)
        return 1;


    return lsqlStringCmp(s1, lsqlStringPtr(s2));
}

