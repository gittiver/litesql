#include "litesql.h"
#include <stdio.h>
int main(int argc, char** argv) {
    lsqlDbDef dbDef;
    int ret = lsqlOpenDbDef(&dbDef, argv[1]);
    printf("Ret: %d\n", ret);

    lsqlCloseDbDef(&dbDef);
    return 0;
}
