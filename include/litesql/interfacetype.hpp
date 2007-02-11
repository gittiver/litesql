/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_interfacetype_hpp
#define _litesql_interfacetype_hpp

#include "litesql/dbtypes.hpp"
#include "litesql/fieldtype.hpp"
#include "litesql/relationtype.hpp"
#include "litesql/relationhandletype.hpp"

namespace litesql {
    
    class InterfaceType {
    public:
        typedef std::vector<RelationType*> Relations;
        typedef std::vector<RelationHandleType*> RelationHandles;

    private:
        Relations       relations;
        RelationHandles handles;
        Fields          fields;

    public:

        InterfaceType(db::Table* table,
                   db::Sequence* sequence,
                   InterfaceType* parent,
                   const Fields& fields,
                   const Relations& relations,
                   const RelationHandles& handles);
    };
        
}
#endif


