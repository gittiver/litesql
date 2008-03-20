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
#include "litesql.h"
#include <malloc.h>
#include <assert.h>

int main(int argc, char** argv) {
    lsqlString s;
    char* m;
    int i;

    lsqlStringNew(&s);

    lsqlStringCopy(&s, "test");
    assert( lsqlStringSize(&s) == 4 );


    lsqlStringDelete(&s);

    lsqlStringNew(&s);
    assert( lsqlStringSize(&s) == 0 );
    lsqlStringDelete(&s);

    m = (char*) malloc(1024);
    for (i = 0; i < 1023; i++)
        m[i] = 'a';
    m[1023] = '\0';

    lsqlStringNew(&s);
    lsqlStringCopy(&s, m);

    assert( lsqlStringSize(&s) == 1023 );
    lsqlStringDelete(&s);

    free(m);

    m = (char*) malloc(123321);
    for (i = 0; i < 123320; i++)
        m[i] = 'b';
    m[123320] = '\0';

    lsqlStringNew(&s);
    lsqlStringCopy(&s, m);

    assert( lsqlStringSize(&s) == 123320 );
    lsqlStringDelete(&s);

    free(m); 


    return 0;
}
