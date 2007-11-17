#ifndef _litesql_split_h_
#define _litesql_split_h_

#include "litesql/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    lsqlString* parts;
    size_t partsSize;
    size_t used;
} lsqlSplit; 

int lsqlSplitNew(lsqlSplit* s);

void lsqlSplitDelete(lsqlSplit* s);

int lsqlSplitReserve(lsqlSplit* s, size_t size);

int lsqlSplitResize(lsqlSplit* s, size_t size);

int lsqlSplitCopy(lsqlSplit* s, int i, const lsqlString* src);
        
int lsqlSplitJoin(const lsqlSplit* s, 
                  lsqlString* dst, 
                  const lsqlString* delim);

int lsqlSplitSlice(lsqlSplit* dst, const lsqlSplit* src,  
                   int start, int end);

int lsqlSplitAdd(lsqlSplit* dst, const lsqlString* src);

int lsqlSplitSize(const lsqlSplit* s);

lsqlString* lsqlSplitElem(lsqlSplit* dst, int i);
#ifdef __cplusplus
}
#endif 
#endif
