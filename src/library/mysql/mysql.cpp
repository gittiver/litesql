/* LiteSQL - MySQL-backend implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "config.h"
#include "mysql.hpp"

#include <string>

#ifdef HAVE_LIBMYSQLCLIENT

#ifdef WIN32
#include <winsock2.h>				/* For windows */
#endif

#include <mysql.h>

using namespace litesql;
using namespace std;
using std::string;
using std::vector;

/** MySQL - result */
class MySQL::Result : public Backend::Result {
  MYSQL_RES * res;
  size_t nFields;
public:
  Result(MYSQL_RES * _res) : res(_res) 
  {     nFields = (!res) ? 0 : mysql_num_fields(res);  };
  
  virtual ~Result();
  virtual size_t fieldNum() const;
  virtual Record fields() const;
  virtual size_t recordNum() const;
  virtual Records records() const;
  const Records& recordsRef() const;
};

MySQL::Result::~Result() {
    mysql_free_result(res);
}
size_t MySQL::Result::fieldNum() const {
  return nFields;    
}

Record MySQL::Result::fields() const {
    if (!res)
        return Record();
    MYSQL_FIELD * flds = mysql_fetch_fields(res);
    size_t numFields = fieldNum(); 
    Record rec(numFields);
    for (size_t i = 0; i < numFields ; i++)
        rec.push_back(flds[i].name);
    return rec;
}

size_t MySQL::Result::recordNum() const {
    if (!res)
        return 0;
    return mysql_num_rows(res);
}  
Records MySQL::Result::records() const {
    if (!res)
        return Records();
    Records recs;

    recs.resize(recordNum());
    mysql_data_seek(res, 0);
    size_t fieldnum = fieldNum();
    for (size_t i = 0; i < recordNum(); i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (!row)
            break;
        recs[i].reserve(fieldnum);
        for (size_t i2 = 0; i2 < fieldnum; i2++) {
            recs[i].push_back(row[i2] ? row[i2] : "NULL");
        }
    }
    return recs;
}

MySQL::MySQL(const std::string& connInfo) {
    vector<string> params = split(connInfo,";");
    host = "localhost";
    int port = 0;
    for (size_t i = 0; i < params.size(); i++) {
        vector<string> param = split(params[i], "=");
        if (param.size() == 1)
            continue;
        if (param[0] == "host")
            host = param[1];
        else if (param[0] == "database")
            database = param[1];
        else if (param[0] == "password")
            passwd = param[1];
        else if (param[0] == "user")
            user = param[1];
        else if (param[0] == "port")
            port = atoi(param[1]);
    }
    conn = new MYSQL;
    mysql_init(conn);
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), passwd.c_str(),
              database.c_str(), port, NULL, 0)) {
        throw DatabaseError(mysql_error(conn));
    }
}

MySQL::~MySQL() {
    mysql_close(conn);
    delete conn;
}

bool MySQL::supportsSequences() const {
    return false;
}
string MySQL::getRowIDType() const {
    return "INTEGER PRIMARY KEY AUTO_INCREMENT";
}
string MySQL::getInsertID() const {
    return toString(mysql_insert_id(conn));
}
void MySQL::begin() const {
  delete execute("BEGIN");
}
void MySQL::commit() const {
  delete execute("COMMIT");
}
void MySQL::rollback() const {
  delete execute("ROLLBACK");
}

Backend::Result* MySQL::execute(const string& query) const {
    if (mysql_real_query(conn, query.c_str(), query.size())) {
        throw SQLError(mysql_error(conn));    
    }
    return new Result(mysql_store_result(conn));
}

/** MySQL - cursor, Note: because MySQL-does not support multiple
  * cursors in single connection, this class actually makes one connection
  * per cursor */
class MySQL::Cursor : public Backend::Cursor {
        const MySQL * const pBackend; 
        MYSQL_RES * res;
        size_t fieldNum;
    public:
        Cursor(const MySQL* const backend, string q);
        virtual ~Cursor();

        virtual Record fetchOne();
};

MySQL::Cursor::Cursor(const MySQL * const backend, string q)
     : pBackend(backend),
       res(NULL) 
    {
      if (mysql_real_query(pBackend->conn, q.c_str(), q.size())) {
        throw SQLError(mysql_error(pBackend->conn));
    }
      // TODO from here exclusive usage of connection needed
    res = mysql_use_result(pBackend->conn);
    fieldNum = mysql_num_fields(res);
}
                                               
Record MySQL::Cursor::fetchOne() {
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row)
    {
      // TODO from here non exclusive of connection possible
      return Record();
    }
    Record rec(fieldNum);
    for (size_t i = 0; i < fieldNum; i++)
        rec.push_back(row[i] ? row[i] : "NULL");
    return rec;
}
MySQL::Cursor::~Cursor() {
    while (mysql_fetch_row(res) != NULL) {}
      // TODO from here non exclusive of connection possible
    mysql_free_result(res);
}

Backend::Cursor* MySQL::cursor(const string& query) const {
  return new Cursor(this,query);       
}

#endif
