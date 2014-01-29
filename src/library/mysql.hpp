/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file mysql.hpp
 * MySQL MySQL::Cursor MySQL::Backend */
#ifndef litesql_mysql_hpp
#define litesql_mysql_hpp

#ifdef HAVE_LIBMYSQLCLIENT


#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

typedef struct st_mysql MYSQL;

namespace litesql {

/** MySQL - backend */
class MySQL : public Backend {
    MYSQL* conn;
  std::string host, user, passwd, database;
    int port;
public:
  class Cursor;
  class Result;

  MySQL(const std::string& connInfo);
    virtual ~MySQL();

    virtual bool supportsSequences() const;
    virtual std::string getRowIDType() const;
    virtual std::string getInsertID() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
  Backend::Result* execute(const std::string& query) const;
  Backend::Cursor* cursor(const std::string& query) const;
};
}
#endif // #ifndef litesql_mysql_hpp
#endif // #ifdef HAVE_LIBMYSQLCLIENT
