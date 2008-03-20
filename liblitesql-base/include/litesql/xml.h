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
} lsqlFldTriggerDef;

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

    lsqlFldTriggerDef* triggers;
    size_t triggersSize;

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

} lsqlObjTriggerDef;

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

    lsqlFldTriggerDef* triggers;
    size_t triggersSize;

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

    lsqlBool owner;

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

    lsqlObjTriggerDef* triggers;
    size_t triggersSize;

    lsqlRelateDef* relates;
    size_t relatesSize;

    lsqlTableDef* table;
    lsqlSequenceDef* sequence;
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

    lsqlTableDef* table;
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


