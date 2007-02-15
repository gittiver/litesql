/* LiteSQL - Database types
 *  
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql.hpp"
namespace litesql {

    using namespace std;
    namespace db {

        string Table::name() const {
            return name_;
        }

        Table* Field::table() const {
            return table_;
        }

        string Field::name() const {
            return name_;
        }
    }


    


}
