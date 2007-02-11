/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_relationtype_hpp
#define _litesql_relationtype_hpp

#include "litesql/dbtypes.hpp"
#include "litesql/fieldtype.hpp"

class ObjectType;

namespace litesql {

    class RelationType {
    public:
        typedef std::vector<ObjectType*> Objects;
        typedef std::vector<FieldType*> Fields;

    private:
        db::Table*    table;
        Objects       objects;
        Fields        fields;

    public:
        RelationType(db::Table* table,
                     const Objects& objects,
                     const Fields& fields);
    };
        
}
#endif


