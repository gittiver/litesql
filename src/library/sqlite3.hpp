/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file sqlite3.hpp
 * SQLite3 SQLite3::Result SQLite3::Cursor */
#ifndef _litesql_sqlite3_hpp
#define _litesql_sqlite3_hpp

#ifdef HAVE_LIBSQLITE3

#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

#include <string>
struct sqlite3;
struct sqlite3_stmt;

namespace litesql {
    /** SQLite3 - backend */
    class SQLite3 : public Backend {

        sqlite3 *db;
        mutable bool transaction;
        std::string beginTrans;

    protected:
        void throwError(int status) const;

    public:
        class Cursor;
        class Result;

        SQLite3(const std::string& database);
        virtual ~SQLite3();

        virtual bool supportsSequences() const;
        virtual std::string getInsertID() const;
        virtual void begin() const;
        virtual void commit() const;
        virtual void rollback() const;

        Backend::Result* execute(const std::string& query) const;
        Backend::Cursor* cursor(const std::string& query) const;
    };
}
#endif // #ifdef HAVE_LIBSQLITE3
#endif // #ifndef _litesql_sqlite3_hpp
