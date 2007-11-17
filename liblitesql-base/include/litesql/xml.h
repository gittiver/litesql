#ifndef _litesql_xmltypes_h_
#define _litesql_xmltypes_h_

#include "litesql/base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    lsqlString* xmlFile;
    unsigned int line;
} lsqlXmlPos;


typedef struct {
    lsqlXmlPos pos;
    lsqlString name;
    lsqlString value;
    lsqlString backend;
} lsqlOptionDef;


typedef struct {
    lsqlXmlPos pos;
    lsqlString as;
    lsqlString target;
} lsqlReprDef;

typedef struct {
    lsqlXmlPos pos;
    lsqlString as;
    lsqlString backend;
} lsqlStoreDef;

typedef struct {
    lsqlXmlPos pos;
    lsqlString functionName;
    lsqlString param;
    int (*function)(lsqlString* value, lsqlString* param);
} lsqlFldCheckDef;

typedef struct {
    lsqlXmlPos pos;
    lsqlString name;
    lsqlString value;
} lsqlValueDef;

typedef struct {
    lsqlXmlPos pos;
    lsqlString name;

    lsqlValueDef* values;
    size_t valuesSize;

    lsqlReprDef* representations;
    size_t representationsSize;

    lsqlStoreDef* stores;
    size_t storesSize;

    lsqlFldCheckDef* checks;
    size_t checksSize;

} lsqlTypeDef;

typedef struct {
    lsqlString name;
    lsqlTypeDef* type;
} lsqlTableFldDef;

typedef struct {
    lsqlString name;

    lsqlTableFldDef* fields;
    size_t fieldsSize;

} lsqlTableDef;

typedef struct {

    lsqlString name;
} lsqlSequenceDef;


typedef struct {
    lsqlXmlPos pos;
    lsqlBool onCreate;
    lsqlBool onDelete;
    lsqlBool onUpdate;
    lsqlBool onLink;
    lsqlBool onUnlink;
              
    lsqlString functionName;
    
    lsqlString param;

    int (*function)(void* obj, lsqlString* param);

} lsqlObjCheckDef;

typedef struct {
    lsqlXmlPos pos;
    lsqlString name;
    lsqlString dbName;
    lsqlString typeName;
    lsqlString defaultValue;
    lsqlBool indexed;
    lsqlBool unique;

    lsqlTypeDef* type;

    lsqlValueDef* values;
    size_t valuesSize;

    lsqlFldCheckDef* checks;
    size_t checksSize;

    size_t offset;
} lsqlFldDef;

typedef struct {
    lsqlXmlPos pos;
    lsqlString name;

    lsqlString type;
    lsqlString defaultValue;

    lsqlBool isConst;

} lsqlParamDef;

typedef struct  {
    lsqlXmlPos pos;

    lsqlString name;

    lsqlString returnType;

    lsqlBool isConst;

    lsqlParamDef* params;
    size_t paramsSize;

} lsqlMtdDef;

struct lsqlObjDef;

typedef struct {
    lsqlXmlPos pos;

    lsqlString name;

    lsqlMtdDef* methods;
    size_t methodsSize;

    struct lsqlObjDef* implementations;
    size_t implementationsSize;

} lsqlIfaceDef;


typedef struct {
    lsqlXmlPos pos;

    lsqlString name;
    lsqlFldDef* field;
} lsqlIdxFldDef;

typedef struct {
    lsqlXmlPos pos;

    lsqlBool unique;

    lsqlIdxFldDef* fields;
    size_t fieldsSize;
} lsqlIdxDef;

typedef struct {
} lsqlHandleDef;


struct lsqlObjDef;
typedef struct {
    lsqlXmlPos pos;

    lsqlString objectName;
    lsqlString interfaceName;

    struct lsqlObjDef* object;
    struct lsqlIfaceDef* interface;

    lsqlBool unique;
    
    lsqlString handleName;

    lsqlString remoteHandle;

    lsqlHandleDef* handle;

} lsqlRelateDef;

typedef struct {
    lsqlXmlPos pos;

    lsqlString interfaceName;

    lsqlIfaceDef* interface;
} lsqlImplDef;

typedef struct lsqlObjDef {
    lsqlXmlPos pos;

    lsqlString name;
    lsqlBool temporary;
    lsqlString inherits;

    lsqlFldDef* fields;
    size_t fieldsSize;

    lsqlMtdDef* methods;
    size_t methodsSize;

    lsqlIdxDef* indices;
    size_t indicesSize;

    lsqlOptionDef* options;
    size_t optionsSize;

    lsqlObjCheckDef* checks;
    size_t checksSize;

    lsqlRelateDef* relates;
    size_t relatesSize;

    lsqlTableDef* table;
    struct lsqlObjDef* parent;

    lsqlImplDef* implements;
    size_t implementsSize;

} lsqlObjDef;

typedef struct {
    lsqlXmlPos pos;

    lsqlString name;
    lsqlString id;

    lsqlRelateDef* relates;
    size_t relatesSize;

    lsqlFldDef* fields;
    size_t fieldsSize;

    lsqlIdxDef* indices;
    size_t indicesSize;

    lsqlOptionDef* options;
    size_t optionsSize;   
} lsqlRelDef;



typedef struct {

    lsqlString name;
    lsqlString namespace_;
    lsqlString include;

    lsqlOptionDef* options;
    size_t optionsSize;   

    lsqlIfaceDef* interfaces;
    size_t interfacesSize;

    lsqlObjDef* objects;
    size_t objectsSize;

    lsqlRelDef* relations;
    size_t relationsSize;

    lsqlTypeDef* types;
    size_t typesSize;

    lsqlTableDef* tables;
    size_t tablesSize;

    lsqlSequenceDef* sequences;
    size_t sequencesSize;

    lsqlString* xmlFiles;
    size_t xmlFilesSize;
} lsqlDbDef;

typedef void (*lsqlErrCallback)(const char*);

int lsqlOpenDbDef(lsqlDbDef* def, const char* path, lsqlErrCallback errCb);
void lsqlCloseDbDef(lsqlDbDef* def);

#ifdef __cplusplus
}        
#endif

#endif


