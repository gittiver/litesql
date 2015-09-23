/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file postgresql.hpp
 * PostgreSQL PostgreSQL::Result PostgreSQL::Cursor */
#ifndef _litesql_postgresql_hpp
#define _litesql_postgresql_hpp
#ifdef HAVE_LIBPQ
#include "libpq-fe.h"
#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

#include <string>
namespace litesql {
using namespace std;
/** PostgreSQL - backend */
class PostgreSQL : public Backend {
    PGconn *conn;
    mutable bool transaction;
public:
    /** PostgreSQL - result */
    class Result : public Backend::Result {
        PGresult *res;
    public:
        Result(PGresult * r) : res(r) {}
        virtual ~Result();
        virtual size_t fieldNum() const;
        virtual Record fields() const;
        virtual size_t recordNum() const;
        virtual Records records() const;
    };
    /** PostgreSQL - cursor
     * Note: cursors can declared only in transactions so this will 
     * silently begin a transaction */
    class Cursor : public Backend::Cursor {
        const PostgreSQL& pq;
        static int sid;
        static size_t cacheSize;
        std::string name;
        Records cache;
        size_t cachePos;
    public:
        virtual void setCacheSize(int v);
        Cursor(const PostgreSQL& p, std::string q);
        virtual Record fetchOne();
        virtual ~Cursor();
    };
    PostgreSQL(const std::string& connInfo);
    virtual bool supportsSequences() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
    Backend::Result* execute(const std::string& query) const;
    Backend::Cursor* cursor(const std::string& query) const;
	string getSQLType(AT_field_type fieldType, const string& length) const;

    virtual ~PostgreSQL();
};
}
#endif
#endif
