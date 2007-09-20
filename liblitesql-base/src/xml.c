#include "litesql.h"
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>


typedef struct {
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;
    xmlNode* root;

    lsqlDbDef* db;
} Parser;

typedef struct {
    const char* name;
    void** array;
    size_t* size;
    size_t elemSize;
    int (*parseFunc)(Parser*, xmlNode*, void*);
} XmlParseDef;



static int getAttr(lsqlString* dst, xmlNode* node, const char* attr) {
    xmlChar* data = xmlGetProp(node, (unsigned char*) attr);
    if (!data)
        return 0;

    if (lsqlStringCopy(dst, (char*) data)) {
        xmlFree(data);
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
    memset(*elem, 0, sizeof(elemSize));
    return 0;
}

static int forNodes(Parser* p, void* ctx, xmlNode* first, 
                    int (*func)(Parser*, void*, xmlNode*)) {

    xmlNode* node = first;
    while (node != NULL) {
        int ret = func(p, ctx, node);
        if (ret) 
            return ret;
        
        node = node->next;
    }
    return 0;
}

static int parseDefs(Parser* p, xmlNode* node, XmlParseDef* defs) {
    size_t i;
    for (i = 0; defs[i].name; i++) {

        printf("testing %s == %s\n", defs[i].name, node->name);
        if (xmlStrcmp(node->name, (const xmlChar*) defs[i].name) == 0) {
            void* ptr = NULL;

            int ret = addToArray(defs[i].array, defs[i].size, 
                                 defs[i].elemSize, &ptr);
            printf("%d \n", ret);
            if (ret)
                return ret;

            return defs[i].parseFunc(p, node, ptr);
        }
    }
    return 0;
}

static int parseFldDef(Parser* p, xmlNode* node, void* ptr) { 
    printf("TODO: parseFldDef\n");
    return 0;
}
static int parseMtdDef(Parser* p, xmlNode* node, void* ptr) { 
    printf("TODO: parseMtdDef\n");

    return 0;

}
static int parseIdxDef(Parser* p, xmlNode* node, void* ptr) { 
    printf("TODO: parseIdxDef\n");

    return 0;


}
static int parseOptionDef(Parser* p, xmlNode* node, void* ptr) { 
    printf("TODO: parseOptionDef\n");

    return 0;

}
static int parseImplDef(Parser* p, xmlNode* node, void* ptr) { 
    printf("TODO: parseImplDef\n");

    return 0;

}
static int parseObjCheckDef(Parser* p, xmlNode* node, void* ptr) { 
    printf("TODO: parseObjCheckDef\n");

    return 0;

}
static int parseObjDefNodes(Parser* p, void* ctx, xmlNode* node) {

    lsqlObjDef* obj = (lsqlObjDef*) ctx;
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
            sizeof(lsqlIfaceDef), parseImplDef },
        {"check", (void**) &obj->checks, &obj->checksSize,
            sizeof(lsqlObjCheckDef), parseObjCheckDef },
        {NULL, NULL, NULL, 0, NULL} };
    

    return parseDefs(p, node, defs);

}

static int parseObjDef(Parser* p, xmlNode* node, void* ptr) {
    int ret = 0;
    lsqlObjDef* obj = (lsqlObjDef*) ptr;

    ret = forNodes(p, obj, node->children, parseObjDefNodes);
    if (ret)
        return ret;


    ret |= getAttr(&obj->name, node, "name");
    ret |= getBool(&obj->temporary, node, "temporary");
    ret |= getAttr(&obj->inherits, node, "inherits");
    printf("Parsing obj def %s\n", node->name);

    return ret;
}

static int parseRelDefNodes(Parser* p, void* ctx, xmlNode* node) {

    lsqlObjDef* rel = (lsqlObjDef*) ctx;
    XmlParseDef defs[] = {
/*Ã       {"relate", (void**) &rel->relates, &rel->relatesSize,
            sizeof(lsqlRelateDef), parseRelateDef },  TODO*/
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
    lsqlObjDef* rel = (lsqlObjDef*) ptr;

    ret = forNodes(p, rel, node->children, parseObjDefNodes);
    if (ret)
        return ret;


/*Ã   ret |= getAttr(&obj->name, node, "name");
    ret |= getBool(&obj->temporary, node, "temporary");
    ret |= getAttr(&obj->inherits, node, "inherits");
    printf("Parsing rel def %s\n", node->name);
*/
    return ret;
}


static int parseDbDefNodes(Parser* p, void* ctx, xmlNode* node) {

    int ret;
    XmlParseDef defs[] = {
        {"object", (void**) &p->db->objects, &p->db->objectsSize,
            sizeof(lsqlObjDef), parseObjDef },
        {"relation", (void**) &p->db->relations, &p->db->relationsSize,
            sizeof(lsqlRelDef), parseRelDef },
        {NULL, NULL, NULL, 0, NULL} };
    
    ret = parseDefs(p, node, defs);
    if (ret)
        return ret;
    
    if (xmlStrcmp(node->name, (const xmlChar*) "include") == 0) {
        /* TODO: */
        
    }
    return 0;
}

int lsqlOpenDbDef(lsqlDbDef* def, const char* path) {
    int ret;

    LIBXML_TEST_VERSION
    Parser parser; 

    memset(&parser, 0, sizeof(Parser));
    parser.ctxt = xmlNewParserCtxt();
    parser.db = def;

    if (parser.ctxt == NULL) 
        return LSQL_MEMORY;

    parser.doc = xmlCtxtReadFile(parser.ctxt, path, NULL, 
                                 XML_PARSE_DTDATTR | XML_PARSE_DTDVALID
                                 | XML_PARSE_COMPACT | XML_PARSE_NOENT);

    if (parser.ctxt->valid == 0) {
        ret = LSQL_XML;
        goto finish;
    }
    memset(def, 0, sizeof(lsqlDbDef));

    lsqlStringNew(&def->name);
    lsqlStringNew(&def->namespace_);
    lsqlStringNew(&def->include);

    parser.root = xmlDocGetRootElement(parser.doc);

    ret |= getAttr(&def->name, parser.root, "name");
    ret |= getAttr(&def->namespace_, parser.root, "namespace");
    ret |= getAttr(&def->include, parser.root, "include");

    

    ret |= forNodes(&parser, NULL, parser.root->children, parseDbDefNodes);


finish:

    xmlFreeDoc(parser.doc);
    xmlFreeParserCtxt(parser.ctxt);
    xmlCleanupParser();
    return ret;
}

void lsqlCloseDbDef(lsqlDbDef* def) {
    lsqlStringDelete(&def->name);
    lsqlStringDelete(&def->namespace_);
    lsqlStringDelete(&def->include);
}
