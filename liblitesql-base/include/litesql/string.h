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


