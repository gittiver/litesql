#include "litesql.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
int main(int argc, char** argv) {
    char buf[32];
    lsqlString s;

    lsqlStringNew(&s);
    lsqlStringCopy(&s, "lsqlString");
    lsqlSnprintf(buf, 32, "%d '%s'%d%S%d", 654,
            "testing", -123, &s, 1234567890);
    lsqlStringDelete(&s);
    assert(strcmp(buf, "654 'testing'-123lsqlString1234") == 0);
    return 0;
}
