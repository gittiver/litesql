#ifndef _litesql_sql_hpp_
#define _litesql_sql_hpp_

#include "litesql/backend.hpp"

namespace litesql {
    namespace sql {
        class SQL {
            public:
                virtual void execute(Backend*) const=0;        
        };
    }
}
#endif
