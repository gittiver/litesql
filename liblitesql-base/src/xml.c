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
