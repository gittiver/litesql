/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_objecttype_hpp
#define _litesql_objecttype_hpp

#include "litesql/dbtypes.hpp"
#include "litesql/fieldtype.hpp"
#include "litesql/relationtype.hpp"
#include "litesql/relationhandletype.hpp"

namespace litesql {
    
    class ObjectType {
    public:
        typedef std::vector<FieldType*> Fields;
        typedef std::vector<RelationType*> Relations;
        typedef std::vector<RelationHandleType*> RelationHandles;

    private:
        db::Table*      table;
        db::Sequence*   sequence;
        ObjectType*     parent;
        Relations       relations;
        RelationHandles handles;
        Fields          fields;

    public:

        ObjectType(db::Table* table,
                   db::Sequence* sequence,
                   ObjectType* parent,
                   const Fields& fields,
                   const Relations& relations,
                   const RelationHandles& handles);
    };
        
}
#endif


