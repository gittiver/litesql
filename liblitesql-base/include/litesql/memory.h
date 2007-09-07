#ifndef _litesql_memory_h
#define _litesql_memory_h

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* lsqlMalloc(size_t bytes);
void* lsqlRealloc(void* ptr, size_t bytes);
void lsqlFree(void* ptr);

void lsqlSetAllocator(void*(*malloc)(size_t), 
                        void*(*realloc)(void*, size_t),
                        void(*free)(void*));

#ifdef __cplusplus
} // extern "C"
#endif

#endif
