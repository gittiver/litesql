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
#include <stdlib.h>
#include <stdio.h>
typedef struct {
    lsqlDbDef* db;
    lsqlErrCallback errCb;
} Context;

typedef struct {
    const char* name;
    void* array;
    size_t arraySize, elemSize;
    lsqlXmlPos* (*getPos)(void*);
    lsqlString* (*getProperty)(void*);
} GetPropertyDef;

static int matches(lsqlString* s, const char** words) {
    const char** word = words;
    for (; *word; word++)
        if (lsqlStringCmp(s, *word) == 0)
            return 1;

    return 0;
}

static int empty(lsqlString* s) {
    return lsqlStringSize(s) == 0;
}
static int validId(lsqlString* s) {
    return !empty(s);
}

static int err(int errcode, Context* ctx, lsqlXmlPos* pos, const char* msg) {
    char buf[1024];
    snprintf(buf, 1024, "%s:%d, %s", lsqlStringPtr(pos->xmlFile),
             pos->line, msg);
    ctx->errCb(buf);
    return errcode;
}
static int err2(int errcode, Context* ctx, lsqlXmlPos* pos, const char* msg,
                const char* data) {
    char buf[1024];
    snprintf(buf, 1024, "%s '%s'", msg, data);
    return err(errcode, ctx, pos, buf);
}
static int checkNodes(Context* ctx, void* array, size_t arraySize,
                      size_t elemSize,
                      int (*func)(Context*, void*)) {
    size_t i;
    for (i = 0; i < arraySize; i++) {

        void* ptr = ((char*) array) + elemSize * i;
        int ret = func(ctx, ptr);
        if (ret)
            return ret;
    }
    return 0;
}

static int checkOption(Context* ctx, void* ptr) {
    char* validOptions[] = {
        "storageEngine",
        NULL
    };

    char* validStorageEngines[] = {
        "myisam", "innodb", "merge", "memory", "berkeleydb", 
        "federated", "archive", "csv", "blackhole" 
    };
    lsqlOptionDef* o = (lsqlOptionDef*) ptr;

    if (empty(&o->name)) 
        return err(LSQL_XMLDATA, ctx, &o->pos, "option.name cannot be empty");
    if (!matches(&o->name, validOptions)) 
        return err2(LSQL_XMLDATA, ctx, &o->pos, 
                   "option.name is not a valid option", 
                   lsqlStringPtr(&o->name));

    if (lsqlStringCmp(&o->name, "storageEngine") == 0) {
        if (!matches(&o->value, validStorageEngines))
            return err2(LSQL_XMLDATA, ctx, &o->pos, 
                       "option.value is not a valid storage engine : ",
                       lsqlStringPtr(&o->value));
        if (lsqlStringCmp(&o->backend, "mysql") != 0)
            return err(LSQL_XMLDATA, ctx, &o->pos,
                       "option.backend, storageEngine-option is only "
                       "supported by mysql-backend");
    }

    return 0;            
}
static int checkInterface(Context* ctx, void* ptr) {

    lsqlIfaceDef* i = (lsqlIfaceDef*) ptr;

    if (!validId(&i->name)) 
        return err(LSQL_XMLDATA, ctx, &i->pos, "interface.name cannot be empty");

    return 0;            
}
static int checkObject(Context* ctx, void *ptr) {
    lsqlObjDef* o = (lsqlObjDef*) ptr;
    int ret = 0;
    ret |= checkNodes(&ctx, (void*) o->options, o->optionsSize, 
                      sizeof(lsqlOptionDef), checkOption);
 
    return ret;
}
static lsqlString* getRelateName(lsqlRelateDef* r) {
    if (lsqlStringSize(&r->objectName) == 0)
        return &r->interfaceName;

    return &r->objectName;
}

static int compareRelate(const void* p1, const void* p2) {
    lsqlRelateDef* r1 = (lsqlRelateDef*) p1;
    lsqlRelateDef* r2 = (lsqlRelateDef*) p2;

    lsqlString* s1 = getRelateName(r1);
    lsqlString* s2 = getRelateName(r2);
    return lsqlStringCmp2(s1, s2);

}
static int checkRelation(Context* ctx, void* ptr) {

    lsqlRelDef* r = (lsqlRelDef*) ptr;
    int ret = 0;

    ret |= checkNodes(&ctx, (void*) r->options, r->optionsSize, 
                      sizeof(lsqlOptionDef), checkOption);
    if (r->relatesSize == 0)

    qsort(r->relates, r->relatesSize, sizeof(lsqlRelateDef),
          compareRelate);

    if (empty(&r->name)) {
        size_t i;
        lsqlSplit s;
        ret = lsqlSplitNew(&s);
        if (ret)
            return ret;
        for (i = 0; i < r->relatesSize; i++)
            lsqlSplitAdd2(&s, getRelateName(&r->relates[i]));
       
        lsqlSplitAdd2(&s, &r->id); 
        lsqlSplitJoin(&s, &r->name, ""); 
        lsqlSplitDelete(&s);

        
    }

    if (!validId(&r->name)) 
        return err2(LSQL_XMLDATA, ctx, &r->pos, 
                   "relation.name not a valid identifier", 
                   lsqlStringPtr(&r->name));

    return ret;            
}
static int checkType(Context* ctx, void* ptr) {
    lsqlTypeDef* t = (lsqlTypeDef*) ptr;
    return 0;
}

static lsqlString* getObjName(void* ptr) {
    return &((lsqlObjDef*) ptr)->name;
}
static lsqlXmlPos* getObjPos(void* ptr) {
    return &((lsqlObjDef*) ptr)->pos;
}

static lsqlString* getRelName(void* ptr) {
    lsqlRelDef* rel = (lsqlRelDef*) ptr;
    return &rel->name;
}

static lsqlString* getOptionName(void* ptr) {
    return &((lsqlOptionDef*) ptr)->name;
}
static lsqlXmlPos* getOptionPos(void* ptr) {
    return &((lsqlOptionDef*) ptr)->pos;
}
static lsqlString* getRelTableName(void* ptr) {
    lsqlRelDef* rel = (lsqlRelDef*) ptr;
    return &rel->table->name;
}

static lsqlXmlPos* getRelPos(void* ptr) {
    return &((lsqlRelDef*) ptr)->pos;
}


static int checkUnique(Context* ctx, GetPropertyDef* defs, 
                       const char* name,
                       lsqlXmlPos* pos, lsqlString* prop) {
    GetPropertyDef* def = defs;
    int num = 0;
    while (def->name) {
        size_t i;
        for (i = 0; i < def->arraySize; i++) {
            void* ptr = ((char*) def->array) + i * def->elemSize;
            lsqlXmlPos* pos2 = def->getPos(ptr);
            lsqlString* prop2 = def->getProperty(ptr);
            if (lsqlStringCmp2(prop, prop2) == 0) {
                num++;
                if (num > 1) {
                    char buf[1024];
                    snprintf(buf, 1024, "'%s' defined again in %s "
                                        "(first: %s:%d in %s)",
                                        lsqlStringPtr(prop), def->name,
                                        lsqlStringPtr(pos->xmlFile),
                                        pos->line, name);
                    return err(LSQL_XMLDATA, ctx, pos2, buf);
                }
            }
        }
        def++;
    }
    return 0;

}

static int areUnique(Context* ctx, GetPropertyDef* defs) {
    GetPropertyDef* def = defs;
    while (def->name) {
        int ret;
        size_t i;
        for (i = 0; i < def->arraySize; i++) {
            void* ptr = ((char*) def->array) + i * def->elemSize;
            lsqlXmlPos* pos = def->getPos(ptr);
            lsqlString* prop = def->getProperty(ptr);
            ret = checkUnique(ctx, defs, def->name, pos, prop);
            if (ret)
                return ret;
        }
        def++;
    }
    return 0;
}

static int checkUniqueness(Context* ctx) {
    int err = 0;
    lsqlDbDef* db = ctx->db;
    GetPropertyDef defs[] = {
        {"object.name", (void*) db->objects, db->objectsSize, 
                     sizeof(lsqlObjDef), getObjPos, getObjName },
        {"relation.name", (void*) db->relations, db->relationsSize, 
                     sizeof(lsqlRelDef), getRelPos, getRelName },
        {"option.name", (void*) db->options, db->optionsSize,
                     sizeof(lsqlOptionDef), getOptionPos, getOptionName },
        {NULL, NULL, 0, 0, NULL}
    };
    err = areUnique(ctx, defs);

    return err;
}

int lsqlCheckDbDef(lsqlDbDef* db, lsqlErrCallback errCb) {
    int ret = 0;
    Context ctx; 
    ctx.db = db;
    ctx.errCb = errCb;
    ret |= checkNodes(&ctx, (void*) db->options, db->optionsSize, 
                      sizeof(lsqlOptionDef), checkOption);
    ret |= checkNodes(&ctx, (void*) db->interfaces, db->interfacesSize,
                      sizeof(lsqlIfaceDef), checkInterface);
    ret |= checkNodes(&ctx, (void*) db->objects, db->objectsSize,
                      sizeof(lsqlObjDef), checkObject);
    ret |= checkNodes(&ctx, (void*) db->relations, db->relationsSize,
                      sizeof(lsqlRelDef), checkRelation);
    ret |= checkNodes(&ctx, (void*) db->types, db->typesSize,
                      sizeof(lsqlTypeDef), checkType);

    ret |= checkUniqueness(&ctx);



    return ret;
}

