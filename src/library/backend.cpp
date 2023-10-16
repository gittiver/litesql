/* LiteSQL - Common routines for backends
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql/backend.hpp"
#include "litesql/string.hpp"

#include "plugin.hpp"
#include "litesql/utils.hpp"
#include "litesql/except.hpp"
#include <iostream>

#ifndef  LITESQL_WITH_BACKEND_PLUGINS

#include "sqlite3/sqlite3.hpp"

#ifdef HAVE_LIBMYSQLCLIENT
#include "mysql/mysql.hpp"
#endif

#ifdef HAVE_LIBPQ
#include "postgresql.hpp"
#endif

#ifdef HAVE_ODBC
#include "odbc_backend.hpp"
#endif


#ifdef HAVE_OCILIB
#include "ocilib_backend.hpp"
#endif
#endif

using namespace litesql;

using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::vector;

string Backend::getSQLType(AT_field_type fieldType,
                           const string& length) const
{
  std::ostringstream oss;
  switch(fieldType) {
    case A_field_type_integer: return "INTEGER";
    case A_field_type_bigint: return "BIGINT";
    case A_field_type_string: 
      oss << "VARCHAR(";
      (length.size()>0)?(oss << length):(oss << "191");
      oss << ")";
      return oss.str();
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
      string fieldString = join(fields[i],",");
        string valueString;
        if (!insertValues[i].empty())
            insertValues[i][0] = id;
      vector<string> valueSplit = insertValues[i];
        for (size_t i2 = 0; i2 < valueSplit.size(); i2++)
            valueSplit[i2] = escapeSQL(valueSplit[i2]);
        valueString = join(valueSplit,",");
        string query = "INSERT INTO " + tables[i] + " (" + fieldString
            + ") VALUES (" + valueString + ")";
        delete execute(query);
        if (!supportsSequences() && id == "NULL") 
            id = getInsertID();
        
    }
    return id;
}

#ifdef  LITESQL_WITH_BACKEND_PLUGINS
class LoadedBackend : public Backend {
public:
  LoadedBackend(Plugin* p, Backend* pBackend)
  : plugin(p)
  , backend(pBackend) {}
  ~LoadedBackend() {
    plugin->destroy(backend);
    backend = nullptr;
  }

  virtual bool supportsSequences() const override {
    return backend->supportsSequences();
  }

  virtual std::string getSQLType(AT_field_type fieldType, const std::string& length="") const override
  { return backend->getSQLType(fieldType,length); }

virtual std::string getCreateSequenceSQL(const std::string& name) const override
  { return backend->getCreateSequenceSQL(name); }
  virtual std::string getSeqSQL(const std::string& sname) const override
  { return backend->getSeqSQL(sname); }

  /** backend may want to set an AUTO_INCREMENT-attribute for table's primary
   key field. this method is to deliver the details to database schema */
  virtual std::string getRowIDType() const override {
    return backend->getRowIDType();
  }

  virtual std::string getInsertID() const override
  { return backend->getInsertID(); }

  virtual void begin() const override
  { backend->begin(); }

  virtual void commit() const override
  { backend->commit(); }

  virtual void rollback() const override
  { backend->rollback(); }

  litesql::Backend::Result *execute(const std::string &query) const override
  { return backend->execute(query); }

  litesql::Backend::Cursor *cursor(const std::string &query) const override
  {return backend->cursor(query);}

  static Backend* load(const char* sharedLib_filename, const string& db_creation_parameter) {
    Backend* lb = nullptr;
    Plugin* plugin = Plugin::load(sharedLib_filename);
    if (plugin)
    {
      Backend* backend = plugin->create(db_creation_parameter);
      if (!backend) {
	 delete plugin;
      }
      else
        lb = new LoadedBackend(plugin,backend);
    }
    return lb;
  }

private:
  shared_ptr<Plugin> plugin;
  Backend* backend;
};

#ifdef CMAKE_SHARED_LIBRARY_PREFIX
#define LIBNAME(name) CMAKE_SHARED_LIBRARY_PREFIX #name CMAKE_SHARED_LIBRARY_SUFFIX 
#else
#ifdef _DEBUG
#define LIBNAME(name) #name "d" CMAKE_SHARED_LIBRARY_SUFFIX 
#else
#define LIBNAME(name) #name CMAKE_SHARED_LIBRARY_SUFFIX 
#endif
#endif 

const char* BACKEND_LIBRARYNAME[5][2] = {
  {"sqlite3",LIBNAME(litesql_backend_sqlite3)},
  {"mysql",LIBNAME(litesql_backend_mysql)},
  {"postgresql",LIBNAME(litesql_backend_postgresql)},
  {"odbc",LIBNAME(litesql_backend_odbc)},
  {"oracle",LIBNAME(litesql_backend_oracle)}
};

static const char* getLibraryName(const char* backendType) {
  const char* libName = nullptr;
  for(size_t i = 0;
      i < 5;
      i++)
  {
    if(!strcmp(backendType, BACKEND_LIBRARYNAME[i][0])) {
      libName = BACKEND_LIBRARYNAME[i][1];
      break;
    }
  }
  return libName;
}
#endif //#ifdef  LITESQL_WITH_BACKEND_PLUGINS

unique_ptr<Backend> Backend::getBackend(const string& backendType,
                                        const string& connInfo) noexcept(false)
{
  Backend* backend=nullptr;
#ifdef  LITESQL_WITH_BACKEND_PLUGINS
  backend = LoadedBackend::load(getLibraryName(backendType.c_str()),connInfo);
    if (!backend) {
      throw DatabaseError("could not load plugin");
	}
#else
  if (backendType == "mysql") {
#ifdef HAVE_LIBMYSQLCLIENT
    backend = new MySQL(connInfo);
#endif
  } else if (backendType == "postgresql") {
#ifdef HAVE_LIBPQ
    backend = new PostgreSQL(connInfo);
#endif
  } else if (backendType == "odbc") {
#ifdef HAVE_ODBC
    backend = new ODBCBackend(connInfo);
#endif
  } else if (backendType=="sqlite3") {
#ifdef HAVE_LIBSQLITE3
    backend = new SQLite3(connInfo);
#endif
  } else if (backendType == "oracle") {
#ifdef HAVE_OCILIB
    backend = new OCILib(connInfo);
#endif
  } else {
    //  backend = nullptr;
  };
#endif //#ifdef  LITESQL_WITH_BACKEND_PLUGINS
  return unique_ptr<Backend>(backend);
}
