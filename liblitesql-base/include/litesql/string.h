#ifndef _litesql_string_h_
#define _litesql_string_h_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* data;
    size_t size, used;
} lsqlString;

int lsqlStringNew(lsqlString* s);
int lsqlStringCopy(lsqlString* dst, const char* src);
int lsqlStringCat(lsqlString* dst, lsqlString* src);
void lsqlStringDelete(lsqlString* s);

#ifdef __cplusplus
} // extern "C"
#endif

#endif


