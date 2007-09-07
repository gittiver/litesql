#ifndef _litesql_split_h_
#define _litesql_split_h_

#include "litesql/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    lsqlString* parts;
    size_t partsSize;
} lsqlSplit; 

int lsqlSplitNew(lsqlSplit* s);

void lsqlSplitDelete(lsqlSplit* s);

int lsqlSplitReserve(lsqlSplit* s, size_t size);

int lsqlSplitResize(lsqlSplit* s, size_t size);

int lsqlSplitCopy(lsqlSplit* s, int i, const lsqlString* src);
        
void lsqlSplitJoin(const lsqlSplit* s, 
                     lsqlString* src, 
                     const lsqlString* delim);

void lsqlSplitSlice(lsqlSplit* dst, const lsqlSplit* src,  
                      int start, int end);

void lsqlSplitInsert(lsqlSplit* dst, const lsqlSplit* src, int i);

int lsqlSplitSize(const lsqlSplit* s);

int lsqlSplitElem(lsqlSplit* dst, const lsqlString* src, int i);
#ifdef __cplusplus
}
#endif 
#endif
