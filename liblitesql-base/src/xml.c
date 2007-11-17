#include "litesql.h"
#include "processxml.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string.h>

typedef struct Parser {
    struct Parser* parent;
    const char* path;
    lsqlErrCallback errCb;
    
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;
    xmlNode* root;
    lsqlString* currentFile;
    lsqlDbDef* db;

} Parser;

typedef struct {
    const char* name;
    void** array;
    size_t* size;
    size_t elemSize;
    int (*parseFunc)(Parser*, xmlNode*, void*);
} XmlParseDef;


static void setPos(lsqlXmlPos* pos, Parser* p,  xmlNode* node) {
    pos->xmlFile = p->currentFile;
    pos->line = node->line;
}
static int getAttr(lsqlString* dst, xmlNode* node, const char* attr) {
    xmlChar* data = xmlGetProp(node, (unsigned char*) attr);

    lsqlStringNew(dst);
    if (!data)
        return 0;
    if (lsqlStringCopy(dst, (char*) data)) {
        xmlFree(data);
        lsqlStringDelete(dst);
        return -1;
    }
    
    xmlFree(data);
    return 0;
}
static int getBool(lsqlBool* dst, xmlNode* node, const char* attr) {
    xmlChar* data = xmlGetProp(node, (unsigned char*) attr);
    if (!data)
        return 0;

    if (xmlStrcmp(data, (const xmlChar*) "true"))
        *dst = 1;
    else
        *dst = 0;
           
    xmlFree(data);
    return 0;
}


static int addToArray(void** array, size_t* size, size_t elemSize, void** elem) {

    (*size)++;
    *array = realloc(*array, elemSize * (*size));
   
    if (!*array) {
        *elem = NULL;
        return LSQL_MEMORY;
    }

    *elem = &((char*)*array)[((*size)-1)*elemSize];    
    memset(*elem, 0, elemSize);
    return 0;
}

static int forNodes(Parser* p, void* ctx, xmlNode* first, 
                    int (*func)(Parser*, xmlNode*, void*)) {
    xmlNode* node = first;
    while (node != NULL) {
        int ret = func(p, node, ctx);
        if (ret) 
            return ret;
        
        node = node->next;
    }
    return 0;
}

static int parseDefs(Parser* p, xmlNode* node, XmlParseDef* defs) {
    size_t i;

    for (i = 0; defs[i].name; i++) {

        if (xmlStrcmp(node->name, (const xmlChar*) defs[i].name) == 0) {
            void* ptr = NULL;

            int ret = addToArray(defs[i].array, defs[i].size, 
                                 defs[i].elemSize, &ptr);
            if (ret)
                return ret;

            return defs[i].parseFunc(p, node, ptr);
        }
    }
    return 0;
}
static int parseValueDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlValueDef* def = (lsqlValueDef*) ptr;
    setPos(&def->pos, p, node);

    ret |= getAttr(&def->name, node, "name");
    ret |= getAttr(&def->value, node, "value");

    return ret;
}

static int parseFldCheckDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlFldCheckDef* chk = (lsqlFldCheckDef*) ptr;
    setPos(&chk->pos, p, node);

    ret |= getAttr(&chk->functionName, node, "function");
    ret |= getAttr(&chk->param, node, "param");
    
    return ret;
}


static int parseFldDefNodes(Parser* p, xmlNode* node, void* ptr) {

    lsqlFldDef* fld = (lsqlFldDef*) ptr;
    XmlParseDef defs[] = {
        {"value", (void**) &fld->values, &fld->valuesSize,
            sizeof(lsqlValueDef), parseValueDef },
        {"check", (void**) &fld->checks, &fld->checksSize,
            sizeof(lsqlFldCheckDef), parseFldCheckDef },
        {NULL, NULL, NULL, 0, NULL} };

    return parseDefs(p, node, defs);    
}
static int parseFldDef(Parser* p, xmlNode* node, void* ptr) { 
    int ret = 0;
    lsqlFldDef* fld = (lsqlFldDef*) ptr;
    setPos(&fld->pos, p, node);

    ret = forNodes(p, fld, node->children, parseFldDefNodes);
    if (ret)
        return ret;

    ret |= getAttr(&fld->name, node, "name");
    ret |= getAttr(&fld->typeName, node, "type");
    ret |= getAttr(&fld->defaultValue, node, "default");
    ret |= getBool(&fld->indexed, node, "indexed");
    ret |= getBool(&fld->unique, node, "unique");

    return ret;
}
static int parseParamDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlParamDef* param = (lsqlParamDef*) ptr;
    setPos(&param->pos, p, node);

    ret |= getAttr(&param->name, node, "name");
    ret |= getAttr(&param->type, node, "type");
    ret |= getAttr(&param->defaultValue, node, "default");
    ret |= getBool(&param->isConst, node, "const");


    return ret;
}
static int parseMtdDefNodes(Parser* p, xmlNode* node, void* ptr) {
    lsqlMtdDef* mtd = (lsqlMtdDef*) ptr;
    XmlParseDef defs[] = {
        {"param", (void**) &mtd->params, &mtd->paramsSize,
            sizeof(lsqlParamDef), parseParamDef }, 
        {NULL, NULL, NULL, 0, NULL} };
    


    return parseDefs(p, node, defs);


}

static int parseMtdDef(Parser* p, xmlNode* node, void* ptr) { 
    int ret = 0;
    lsqlMtdDef* mtd = (lsqlMtdDef*) ptr;
    setPos(&mtd->pos, p, node);

    ret = forNodes(p, mtd, node->children, parseMtdDefNodes);
    if (ret)
        return ret;

    ret |= getAttr(&mtd->name, node, "name");
    ret |= getAttr(&mtd->returnType, node, "returnType");
    ret |= getBool(&mtd->isConst, node, "const");

    return 0;

}

static int parseIdxFldDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlIdxFldDef* fld = (lsqlIdxFldDef*) ptr;
    setPos(&fld->pos, p, node);

    ret |= getAttr(&fld->name, node, "name");

    return ret;
}
static int parseIdxDefNodes(Parser* p, xmlNode* node, void* ptr) {
    lsqlIdxDef* idx = (lsqlIdxDef*) ptr;
    XmlParseDef defs[] = {
        {"indexfield", (void**) &idx->fields, &idx->fieldsSize,
            sizeof(lsqlIdxFldDef), parseIdxFldDef }, 
        {NULL, NULL, NULL, 0, NULL} };   
    return parseDefs(p, node, defs);

}

static int parseIdxDef(Parser* p, xmlNode* node, void* ptr) { 
    int ret = 0;
    lsqlIdxDef* idx = (lsqlIdxDef*) ptr;
    setPos(&idx->pos, p, node);

    ret = forNodes(p, idx, node->children, parseIdxDefNodes);

    if (ret)
        return ret;

    ret |= getBool(&idx->unique, node, "unique");

    return ret;


}
static int parseOptionDef(Parser* p, xmlNode* node, void* ptr) { 
    int ret = 0;

    lsqlOptionDef* opt = (lsqlOptionDef*) ptr;
    setPos(&opt->pos, p, node);

    ret |= getAttr(&opt->name, node, "name");
    ret |= getAttr(&opt->value, node, "value");

    return ret;

}
static int parseImplDef(Parser* p, xmlNode* node, void* ptr) { 
    int ret = 0;
    lsqlImplDef* impl = (lsqlImplDef*) ptr;
    setPos(&impl->pos, p, node);

    ret |= getAttr(&impl->interfaceName, node, "name");    

    return ret;

}
static int parseObjCheckDef(Parser* p, xmlNode* node, void* ptr) { 
    int ret = 0;
    lsqlObjCheckDef* chk = (lsqlObjCheckDef*) ptr;
    setPos(&chk->pos, p, node);

    ret |= getAttr(&chk->functionName, node, "function");
    ret |= getAttr(&chk->param, node, "param");
    ret |= getBool(&chk->onCreate, node, "onCreate");
    ret |= getBool(&chk->onDelete, node, "onDelete");
    ret |= getBool(&chk->onUpdate, node, "onUpdate");
    ret |= getBool(&chk->onLink, node, "onLink");
    ret |= getBool(&chk->onUnlink, node, "onUnlink");
    
    return ret;


    return 0;

}
static int parseObjDefNodes(Parser* p, xmlNode* node, void* ptr) {

    lsqlObjDef* obj = (lsqlObjDef*) ptr;

    XmlParseDef defs[] = {
        {"field", (void**) &obj->fields, &obj->fieldsSize,
            sizeof(lsqlFldDef), parseFldDef }, 
        {"method", (void**) &obj->methods, &obj->methodsSize,
            sizeof(lsqlMtdDef), parseMtdDef },
        {"index", (void**) &obj->indices, &obj->indicesSize,
            sizeof(lsqlIdxDef), parseIdxDef },
        {"option", (void**) &obj->options, &obj->optionsSize,
            sizeof(lsqlOptionDef), parseOptionDef },
        {"implements", (void**) &obj->implements, &obj->implementsSize,
            sizeof(lsqlImplDef), parseImplDef },
        {"check", (void**) &obj->checks, &obj->checksSize,
            sizeof(lsqlObjCheckDef), parseObjCheckDef },
        {NULL, NULL, NULL, 0, NULL} };
    

    return parseDefs(p, node, defs);

}

static int parseObjDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlObjDef* obj = (lsqlObjDef*) ptr;
    
    setPos(&obj->pos, p, node);

    ret = forNodes(p, obj, node->children, parseObjDefNodes);
    if (ret)
        return ret;


    ret |= getAttr(&obj->name, node, "name");
    ret |= getBool(&obj->temporary, node, "temporary");
    ret |= getAttr(&obj->inherits, node, "inherits");

    return ret;
}

static int parseRelateDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlRelateDef* r = (lsqlRelateDef*) ptr;
    setPos(&r->pos, p, node);


    ret |= getAttr(&r->objectName, node, "object");
    ret |= getAttr(&r->handleName, node, "handle"); 

    return ret;   
}

static int parseRelDefNodes(Parser* p, xmlNode* node, void* ptr) {

    lsqlRelDef* rel = (lsqlRelDef*) ptr;
    XmlParseDef defs[] = {
        {"relate", (void**) &rel->relates, &rel->relatesSize,
            sizeof(lsqlRelateDef), parseRelateDef },  
        {"field", (void**) &rel->fields, &rel->fieldsSize,
            sizeof(lsqlFldDef), parseFldDef },
        {"index", (void**) &rel->indices, &rel->indicesSize,
            sizeof(lsqlIdxDef), parseIdxDef },
        {"option", (void**) &rel->options, &rel->optionsSize,
            sizeof(lsqlOptionDef), parseOptionDef },
        {NULL, NULL, NULL, 0, NULL} };
    

    return parseDefs(p, node, defs);

}
static int parseRelDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlRelDef* rel = (lsqlRelDef*) ptr;
    setPos(&rel->pos, p, node);

    ret = forNodes(p, ptr, node->children, parseRelDefNodes);
    if (ret)
        return ret;


    ret |= getAttr(&rel->name, node, "name"); 
    ret |= getAttr(&rel->id, node, "id");

    return ret;
}
static int parseIfaceDefNodes(Parser* p, xmlNode* node, void* ptr) {

    lsqlIfaceDef* iface = (lsqlIfaceDef*) ptr;
    XmlParseDef defs[] = {
        {"method", (void**) &iface->methods, &iface->methodsSize,
            sizeof(lsqlMtdDef), parseMtdDef },  
        {NULL, NULL, NULL, 0, NULL} };
    

    return parseDefs(p, node, defs);

}
static int parseIfaceDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlIfaceDef* iface = (lsqlIfaceDef*) ptr;
    setPos(&iface->pos, p, node);

    ret = forNodes(p, ptr, node->children, parseIfaceDefNodes);
    if (ret)
        return ret;
    ret |= getAttr(&iface->name, node, "name");

    return ret;
}

static int parseReprDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlReprDef* repr = (lsqlReprDef*) ptr;
    setPos(&repr->pos, p, node);

    ret |= getAttr(&repr->as, node, "as");
    ret |= getAttr(&repr->target, node, "target");

    return ret;
}

static int parseStoreDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlStoreDef* store = (lsqlStoreDef*) ptr;
    setPos(&store->pos, p, node);

    ret |= getAttr(&store->as, node, "store");
    ret |= getAttr(&store->backend, node, "backend");

    return ret;
}


static int parseTypeDefNodes(Parser* p, xmlNode* node, void* ptr) {

    lsqlTypeDef* type = (lsqlTypeDef*) ptr;
    XmlParseDef defs[] = {
        { "represent", (void**) &type->representations, 
                                &type->representationsSize,
           sizeof(lsqlReprDef), parseReprDef },  
        { "store", (void**) &type->stores, &type->storesSize,
          sizeof(lsqlStoreDef), parseStoreDef },
        { "value", (void**) &type->values, &type->valuesSize,
          sizeof(lsqlValueDef), parseValueDef },
        { "check", (void**) &type->checks, &type->checksSize,
          sizeof(lsqlFldCheckDef), parseFldCheckDef },
        {NULL, NULL, NULL, 0, NULL} };
    

    return parseDefs(p, node, defs);
}


static int parseTypeDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlTypeDef* type = (lsqlTypeDef*) ptr;
    setPos(&type->pos, p, node);

    ret = forNodes(p, ptr, node->children, parseTypeDefNodes);
    if (ret)
        return ret;
    ret |= getAttr(&type->name, node, "name");

    return ret;
}

static int isAlreadyParsed(Parser* p, const char* path) {
    if (strcmp(path, p->path) == 0)
        return 1;
    if (p->parent)
        return isAlreadyParsed(p->parent, path);
    return 0;
}

static int prepareParsing(Parser* p, lsqlDbDef* db, lsqlErrCallback errCb,
                          const char* path, Parser* parent) {
    int ret = 0;
    lsqlString* pathName;
    memset(p, 0, sizeof(Parser));
    p->path = path;
    p->db = db;
    p->errCb = errCb;
    p->ctxt = xmlNewParserCtxt();

    if (p->ctxt == NULL) 
        return LSQL_MEMORY;

    p->doc = xmlCtxtReadFile(p->ctxt, path, NULL, 
                                 XML_PARSE_DTDATTR | XML_PARSE_DTDVALID
                                 | XML_PARSE_COMPACT | XML_PARSE_NOENT);
    if (p->ctxt->valid == 0 || p->doc == NULL) {        
        return LSQL_XML;
    }
    

    
    p->root = xmlDocGetRootElement(p->doc);
    p->parent = parent;
    ret = addToArray((void**) &p->db->xmlFiles, &p->db->xmlFilesSize, 
                     sizeof(lsqlString), (void**) &pathName);
    lsqlStringNew(pathName);
    lsqlStringCopy(pathName, path);
    p->currentFile = pathName;

    return ret;
}

static void finishParsing(Parser* p) {

    xmlFreeDoc(p->doc);
    xmlFreeParserCtxt(p->ctxt);
}

static int parseDbDefNodes(Parser* p, xmlNode* node, void* ptr) {

    int ret;
    lsqlDbDef* db = (lsqlDbDef*) ptr;

    XmlParseDef defs[] = {
        {"object", (void**) &db->objects, &db->objectsSize,
            sizeof(lsqlObjDef), parseObjDef },  
        {"relation", (void**) &db->relations, &db->relationsSize,
           sizeof(lsqlRelDef), parseRelDef },   
        {"interface", (void**) &db->interfaces, &db->interfacesSize,
           sizeof(lsqlIfaceDef), parseIfaceDef }, 
        {"type", (void**) &db->types, &db->typesSize,
           sizeof(lsqlTypeDef), parseTypeDef },
        {"option", (void**) &db->options, &db->optionsSize,
            sizeof(lsqlOptionDef), parseOptionDef },
        {NULL, NULL, NULL, 0, NULL} };
    
    ret = parseDefs(p, node, defs);
    if (ret)
        return ret;
    
    if (xmlStrcmp(node->name, (const xmlChar*) "include") == 0) {
        Parser p2;
        char* path = (char*) xmlGetProp(node, (unsigned char*) "file");
        if (isAlreadyParsed(p, path))  {
            char buf[1024];
            /* TODO: korvaa snprintf */
            snprintf(buf, 1024, "%s:%d, '%s' already included",
                     lsqlStringPtr(p->currentFile),
                     node->line, path);
            p->errCb(buf);
            return LSQL_LOOP;
        }
        ret = prepareParsing(&p2, db, p->errCb, path, p); 
        if (!ret) 
            ret |= forNodes(&p2, db, p2.root->children, 
                            parseDbDefNodes);

        finishParsing(&p2);
    }
    return ret;
}


int lsqlOpenDbDef(lsqlDbDef* def, const char* path, 
                  lsqlErrCallback errCb) {
    int ret;
    
    LIBXML_TEST_VERSION
    Parser parser; 


    memset(def, 0, sizeof(lsqlDbDef));
    ret = prepareParsing(&parser, def, errCb, path, NULL);
    if (ret)
        goto finish;



    ret |= getAttr(&def->name, parser.root, "name");
    ret |= getAttr(&def->namespace_, parser.root, "namespace");
    ret |= getAttr(&def->include, parser.root, "include");

    

    ret |= forNodes(&parser, def, parser.root->children, parseDbDefNodes);


finish:
    finishParsing(&parser);

    if (!ret)
        return lsqlProcessDbDef(def, errCb);

    return ret;
}

void lsqlCloseDbDef(lsqlDbDef* def) {
    lsqlStringDelete(&def->name);
    lsqlStringDelete(&def->namespace_);
    lsqlStringDelete(&def->include);
    xmlCleanupParser();
}
