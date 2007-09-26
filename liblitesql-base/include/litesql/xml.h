#ifndef _litesql_xmltypes_h_
#define _litesql_xmltypes_h_

#include "litesql/base.h"

#ifdef __cplusplus
extern "C" {
#endif

   

typedef struct {
    lsqlString name;
    lsqlString value;
    lsqlString backend;
} lsqlOptionDef;


typedef struct {
    lsqlString as;
    lsqlString target;
} lsqlReprDef;

typedef struct {
    lsqlString as;
    lsqlString backend;
} lsqlStoreDef;

typedef struct {
    lsqlString functionName;
    lsqlString param;
    int (*function)(lsqlString* value, lsqlString* param);
} lsqlFldCheckDef;

typedef struct {
    lsqlString name;
    lsqlString value;
} lsqlValueDef;

typedef struct {
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

    lsqlTableFldDef* fields;
    size_t fieldsSize;

} lsqlTableDef;

typedef struct {
    lsqlString name;
} lsqlSequenceDef;


typedef struct {
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
    lsqlString name;

    lsqlString type;

    lsqlBool isConst;

} lsqlParamDef;

typedef struct  {
    lsqlString name;

    lsqlString returnType;

    lsqlBool isConst;

    lsqlParamDef* params;
    size_t paramsSize;

} lsqlMtdDef;

struct lsqlObjDef;

typedef struct {
    lsqlString name;

    lsqlMtdDef* methods;
    size_t methodsSize;

    struct lsqlObjDef* implementations;
    size_t implementationsSize;

} lsqlIfaceDef;


typedef struct {
    lsqlString name;
    lsqlFldDef* field;
} lsqlIdxFldDef;

typedef struct {
    lsqlBool unique;

    lsqlIdxFldDef* fields;
    size_t fieldsSize;
} lsqlIdxDef;

typedef struct {
} lsqlHandleDef;


struct lsqlObjDef;
typedef struct {

    lsqlString objectName;

    struct lsqlObjDef* object;

    lsqlBool unique;
    
    lsqlString handleName;

    lsqlString remoteHandle;

    lsqlHandleDef* handle;

} lsqlRelateDef;

typedef struct {

    lsqlString interfaceName;

    lsqlIfaceDef* interface;
} lsqlImplDef;

typedef struct lsqlObjDef {
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

    lsqlTableDef* tables;
    size_t tablesSize;

    lsqlSequenceDef* sequences;
    size_t sequencesSize;

} lsqlDbDef;

int lsqlOpenDbDef(lsqlDbDef* def, const char* path);
void lsqlCloseDbDef(lsqlDbDef* def);

#ifdef __cplusplus
}        
#endif

#endif


