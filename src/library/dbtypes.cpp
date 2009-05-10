/* LiteSQL - Database types
 *  
 * The list of contributors at http://litesql.sf.net/ 
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
