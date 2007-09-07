#include "litesql.h"
#include <malloc.h>
static void* lsqlDefaultMalloc(size_t bytes) {
    /* TODO: small-object allocator */
    return malloc(bytes);
}

static void* lsqlDefaultRealloc(void* ptr, size_t bytes) {
    return realloc(ptr, bytes);
}

static void lsqlDefaultFree(void* ptr) {
    free(ptr);
}

static void* (*currentMalloc)(size_t) = lsqlDefaultMalloc;
static void* (*currentRealloc)(void*,size_t) = lsqlDefaultRealloc;
static void (*currentFree)(void*) = lsqlDefaultFree;

void* lsqlMalloc(size_t bytes) {
    return (*currentMalloc)(bytes);
}

void* lsqlRealloc(void*ptr, size_t bytes) {
    return (*currentRealloc)(ptr, bytes);
}


void lsqlFree(void* ptr) {
    (*currentFree)(ptr);
}

void lsqlSetAllocator(void* (*customMalloc)(size_t), 
                      void* (*customRealloc)(void*, size_t),
                      void (*customFree)(void*)) {

    currentMalloc = customMalloc;
    currentRealloc = customRealloc;
    currentFree = customFree;
}
