/* LiteSQL - test-backend-loading
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include "litesql/database.hpp"
#include "plugin.hpp"
/*
*/
using namespace litesql;

bool testBackendLoadingWithValidConnectionInfo(const char* backend_name) {
  const char* connInfo = "database=test-backend-loading.db";
  bool success = false;
  shared_ptr<Backend> backend;
  try {
    backend = Backend::getBackend(backend_name, connInfo);
  } catch(const DatabaseError& e) {
    success = false;
  }
  if (backend)
  {
    success = false;
    backend->execute("");
    success = true;
  }
  system("rm test-backend-loading.db");
  return success;
}

bool testBackendLoadingWithInvalidParameters(const char* backend_name) {
  static const char* connInfo = "param=b1";
  bool success = false;
  shared_ptr<Backend> backend;
  try {
    backend = Backend::getBackend(backend_name, connInfo);
  } catch(const DatabaseError& e) {
    success = true;
  }
  return success;
}

bool testPluginLoading(const char* sharedLib_filename, const char* db_creation_parameter) {
  bool success = false;

  auto plugin = Plugin::load(sharedLib_filename);
  if (plugin)
  {
    litesql::Backend* b1=nullptr;
    try {
      string error;
      b1 = plugin->create(db_creation_parameter);
    } catch (DatabaseError ) {
      success = true;
      // this is ok, because the params are invalid
    }
    plugin->destroy(b1);
    delete plugin;
  }
  return success;
}

int main(int /*argc*/, char * /*argv*/ []) {
  bool success = testPluginLoading("liblitesql_backend_sqlite.dylib", "param=b1");
  success = success && testBackendLoadingWithInvalidParameters("sqlite3");
  success = success && testBackendLoadingWithValidConnectionInfo("sqlite3");
#ifdef HAVE_LIBMYSQLCLIENT
  success &&= testPluginLoading("liblitesql_backend_mysql.dylib", "param=b1");
  success &&= testBackendLoadingWithInvalidParameters("mysql");
#endif

#ifdef HAVE_LIBPQ
  success &&= testPluginLoading("liblitesql_backend_pq.dylib", "param=b1");
#endif

  return success ? 0 : -1;
}


