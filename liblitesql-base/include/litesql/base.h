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
