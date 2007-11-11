#ifndef _litesql_base_h
#define _litesql_base_h

#include <stddef.h>

#include "litesql/string.h"
#include "litesql/split.h"

#define LSQL_ERROR   1      
#define LSQL_MEMORY  2      /* could not resize memory */
#define LSQL_XML     4      /* xml not validated */
#define LSQL_LOOP    8      /* recursive include */
#define LSQL_XMLDATA 16     /* invalid data in attributes */

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
