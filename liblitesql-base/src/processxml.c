#include "litesql.h"

typedef struct {
    lsqlDbDef* db;
    lsqlErrCallback errCb;
} Context;

static int matches(lsqlString* s, const char** words) {
    const char** word = words;
    for (; *word; word++)
        if (lsqlStringCmp(s, *word))
            return 1;

    return 0;
}

static int empty(lsqlString* s) {
    return lsqlStringSize(s) == 0;
}

static int checkOptions(Context* ctx, 
                        lsqlOptionDef* options, 
                        size_t optionsSize) {
    size_t i;

    for (i = 0; i < optionsSize; i++) {

        lsqlOptionDef* o = &options[i];

        if (empty(&o->name))
            return LSQL_XMLDATA;

    }
    return 0;            
}
int lsqlProcessDbDef(lsqlDbDef* def, lsqlErrCallback errCb) {
    int ret = 0;
    Context ctx; 
    ctx.db = def;
    ctx.errCb = errCb;
    
    ret |= checkOptions(&ctx, def->options, def->optionsSize);


    return ret;
}

