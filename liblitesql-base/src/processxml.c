#include "litesql.h"

static int matches(lsqlString* s, const char** words) {
    const char** word = words;
    for (; *word; word++)
        if (lsqlStringCmp(s, *word))
            return 1;

    return 0;
}

static int isValidOption(lsqlString* name) {
    const char* valid[] = {
        "storageEngine",
        NULL
    };
    return matches(name, valid);
}

static int notEmpty(lsqlString* s) {
    return lsqlStringSize(s) != 0;
}

static int checkOptions(lsqlOptionDef* options, size_t optionsSize) {
    size_t i;

    for (i = 0; i < optionsSize; i++) {

        lsqlOptionDef* o = &options[i];

        if (!notEmpty(&o->name))
            return LSQL_XMLDATA;

    }
    return 0;            
}
int lsqlProcessDbDef(lsqlDbDef* def) {
    int ret = 0;
    
    ret |= checkOptions(def->options, def->optionsSize);


    return ret;
}

