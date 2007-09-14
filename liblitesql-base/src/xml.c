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
        if (ret) {
            return ret;
        }
        node = node->next;
    }
    return 0;
}

static int parseObjDef(Parser* p, xmlNode* node, lsqlObjDef* obj) {
    int ret = 0;
    ret |= getAttr(&obj->name, node, "name");
    ret |= getBool(&obj->temporary, node, "temporary");
    ret |= getAttr(&obj->inherits, node, "inherits");

    return ret;
}

static int parseRelDef(Parser* p, xmlNode* node, lsqlRelDef* rel) {
    printf("Parsing rel def %s\n", node->name);
    return 0;
}

static int parseDbDefNodes(Parser* p, void* ctx, xmlNode* node) {
    printf("parseDbDefNodes %s\n", node->name);
    if (xmlStrcmp(node->name, (const xmlChar*) "object") == 0) {
        lsqlObjDef* obj;
        int ret = addToArray((void**) &p->db->objects, &p->db->objectsSize, 
                   sizeof(lsqlObjDef), (void**) &obj);
        if (ret)
            return ret;

        return parseObjDef(p, node, obj);
    
    } else if (xmlStrcmp(node->name, (const xmlChar*) "relation") == 0) {
        lsqlRelDef* rel;
        int ret = addToArray((void**) &p->db->relations, &p->db->relationsSize,
                   sizeof(lsqlRelDef), (void**) &rel);
        if (ret)
            return ret;

        return parseRelDef(p, node, rel);
    
    } else if (xmlStrcmp(node->name, (const xmlChar*) "include") == 0) {
        
        
    }
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

    

    forNodes(&parser, NULL, parser.root->children, parseDbDefNodes);


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
