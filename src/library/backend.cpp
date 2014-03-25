/* LiteSQL - Common routines for backends
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include <map>

#include "compatibility.hpp"
#include "litesql/backend.hpp"
#include "litesql/string.hpp"
#include "litesql/types.hpp"

#ifdef HAVE_LIBMYSQLCLIENT
#include "mysql.hpp"
#endif

#ifdef HAVE_LIBPQ
#include "postgresql.hpp"
#endif

#ifdef HAVE_LIBSQLITE3
#include "sqlite3.hpp"
#endif

#ifdef HAVE_ODBC
#include "odbc_backend.hpp"
#endif


#ifdef HAVE_OCILIB
#include "ocilib_backend.hpp"
#endif

using namespace litesql;
using namespace std;    

string Backend::getSQLType(AT_field_type fieldType, const string& length) const
{
  switch(fieldType) {
    case A_field_type_integer: return "INTEGER";
    case A_field_type_bigint: return "BIGINT";
    case A_field_type_string: return "TEXT";
    case A_field_type_float: return "FLOAT";
    case A_field_type_double: return "DOUBLE";
    case A_field_type_boolean: return "INTEGER";
    case A_field_type_date: return "INTEGER";
    case A_field_type_time: return "INTEGER";
    case A_field_type_datetime: return "INTEGER";
    case A_field_type_blob: return "BLOB";
    default: return "";
  }
}


string Backend::getCreateSequenceSQL(const string& name) const
{
    return "CREATE SEQUENCE " + name + " START 1 INCREMENT 1";
}

string Backend::getSeqSQL(const string& sname) const
{
    string ret="SELECT nextval('"+ sname + "')";
    return ret;
}

string Backend::groupInsert(const Record& tables, 
                            const Records& fields, 
                            const Records& _values, 
                            const string& sequence) const 
{
    string id = _values[0][0];
    Records insertValues = _values;     
    if (supportsSequences() && id == "NULL") {
      Result * r = execute(getSeqSQL(sequence));
      id = r->records()[0][0];
      delete r;
    } 
    for (int i = tables.size()-1; i >= 0; i--) {
      string fieldString = Split::join(fields[i],",");
        string valueString;
        if (!insertValues[i].empty())
            insertValues[i][0] = id;
        Split valueSplit(insertValues[i]);
        for (size_t i2 = 0; i2 < valueSplit.size(); i2++)
            valueSplit[i2] = escapeSQL(valueSplit[i2]);
        valueString = valueSplit.join(",");
        string query = "INSERT INTO " + tables[i] + " (" + fieldString
            + ") VALUES (" + valueString + ")";
        delete execute(query);
        if (!supportsSequences() && id == "NULL") 
            id = getInsertID();
        
    }
    return id;
}

Backend* Backend::getBackend(const string & backendType,const string& connInfo)
{
  Backend* backend;

#ifdef HAVE_LIBMYSQLCLIENT
  if (backendType == "mysql") {
    backend = new MySQL(connInfo);
  } else
#endif
#ifdef HAVE_LIBPQ
    if (backendType == "postgresql") {
      backend = new PostgreSQL(connInfo);
    } else
#endif
#ifdef HAVE_ODBC
      if (backendType == "odbc") {
        backend = new ODBCBackend(connInfo);
      } else
#endif
#ifdef HAVE_LIBSQLITE3
        if (backendType == "sqlite3") {
          backend = new SQLite3(connInfo);
        } else
#endif
#ifdef HAVE_OCILIB
        if (backendType == "oracle") {
          backend = new OCILib(connInfo);
        } else
#endif
        { 
          backend = NULL;
        };
        return backend;
}
