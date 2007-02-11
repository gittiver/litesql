/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_relationhandletype_hpp
#define _litesql_relationhandletype_hpp

#include "litesql/dbtypes.hpp"
#include "litesql/fieldtype.hpp"
#include "litesql/relationtype.hpp"

class ObjectType;

namespace litesql {
    
    class RelationHandleType {
    public:
        typedef std::vector< ObjectType* > DestObjects;

    private:
        ObjectType*   owner;
        RelationType* relation;
        DestObjects   destObjects;

    public:
        RelationHandleType(ObjectType* owner,
                           RelationType* relation,
                           const DestObjects& objects,
                           const Fields& fields);
    };
}
#endif


