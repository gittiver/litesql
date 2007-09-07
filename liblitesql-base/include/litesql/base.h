#ifndef _litesql_base_h
#define _litesql_base_h

#include <stddef.h>

#include "litesql/string.h"
#include "litesql/split.h"

#define LSQL_ERROR  -1
#define LSQL_MEMORY -2
#define LSQL_XML    -3

#ifdef __cplusplus
extern "C" {
typedef bool lsqlBool;
#else
typedef int lsqlBool;
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
