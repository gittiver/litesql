#ifndef _litesql_string_h_
#define _litesql_string_h_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char* data;
} lsqlString;

int lsqlStringNew(lsqlString* s);

int lsqlStringResize(lsqlString* dst, size_t size);

int lsqlStringCopy(lsqlString* dst, const char* src);

int lsqlStringCopy2(lsqlString* dst, const lsqlString* src);

int lsqlStringCat(lsqlString* dst, const lsqlString* src);

size_t lsqlStringSize(const lsqlString* dst);

void lsqlStringDelete(lsqlString* s);

const char* lsqlStringPtr(const lsqlString* s);

int lsqlStringCmp(const lsqlString* s1, const char* s2);

int lsqlStringCmp2(const lsqlString* s1, const lsqlString* s2);

#ifdef __cplusplus
} // extern "C"
#endif

#endif


