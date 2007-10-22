#include "litesql.h"
#include <malloc.h>
#include <assert.h>

int main(int argc, char** argv) {
    lsqlString s;
    char* m;
    int i;

    lsqlStringNew(&s);

    lsqlStringCopy(&s, "test");
    printf("%d\n", lsqlStringSize(&s));
    assert( lsqlStringSize(&s) == 4 );

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
