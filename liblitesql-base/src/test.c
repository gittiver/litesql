#include "litesql.h"
#include <stdio.h>

void printErr(const char* s) {
    fprintf(stderr, "%s\n", s);
}

int main(int argc, char** argv) {
    lsqlDbDef dbDef;
    int ret = lsqlOpenDbDef(&dbDef, argv[1], printErr);

    printf("Ret: %d\n", ret);
    printf("%zu options\n", dbDef.optionsSize);
    printf("%zu interfaces\n", dbDef.interfacesSize);
    printf("%zu relations\n", dbDef.relationsSize);
    printf("%zu objects\n", dbDef.objectsSize);
    printf("%zu types\n", dbDef.typesSize);
    printf("%zu tables\n", dbDef.tablesSize);
    printf("%zu sequences\n", dbDef.sequencesSize);

    lsqlCloseDbDef(&dbDef);
    return 0;
}
