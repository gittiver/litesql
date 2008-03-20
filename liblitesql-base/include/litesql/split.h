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

int lsqlSplitCopy(lsqlSplit* s, int i, const char* src);
int lsqlSplitCopy2(lsqlSplit* s, int i, const lsqlString* src);
        
int lsqlSplitJoin(const lsqlSplit* s, 
                  lsqlString* dst, 
                  const char* delim);

int lsqlSplitJoin2(const lsqlSplit* s, 
                  lsqlString* dst, 
                  const lsqlString* delim);

int lsqlSplitSlice(lsqlSplit* dst, const lsqlSplit* src,  
                   int start, int end);

int lsqlSplitAdd(lsqlSplit* dst, const char* src);
int lsqlSplitAdd2(lsqlSplit* dst, const lsqlString* src);

int lsqlSplitSize(const lsqlSplit* s);

lsqlString* lsqlSplitElem(lsqlSplit* dst, int i);
#ifdef __cplusplus
}
#endif 
#endif
