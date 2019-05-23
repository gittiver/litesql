/* LiteSQL - test-backend-loading
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include "litesql/database.hpp"
#include "plugin.hpp"

#include "config.h"
/*
*/
using namespace litesql;

bool testBackendLoadingWithValidConnectionInfo(const char* backend_name) {
  const char* connInfo = "database=test-backend-loading.db";
  bool success = false;
  {
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
    string error;
    b1 = plugin->create(db_creation_parameter);
    cout << "after create " << b1 << endl;
    success = (b1==nullptr);
    // this is ok, because the params are invalid
    plugin->destroy(b1);
    delete plugin;
  }
  return success;
}

#ifdef CMAKE_SHARED_LIBRARY_PREFIX
#define LIBNAME(name) CMAKE_SHARED_LIBRARY_PREFIX #name CMAKE_SHARED_LIBRARY_SUFFIX 
#else
#define LIBNAME(name) #name CMAKE_SHARED_LIBRARY_SUFFIX 
#endif
int main(int /*argc*/, char * /*argv*/ []) {
  bool success = testPluginLoading(LIBNAME(litesql_backend_sqlite3), "param=b1");
  cout << "testPluginLoading:" << (success ? "OK" : "FAIL") << endl;

  success = success && testBackendLoadingWithInvalidParameters("sqlite3");
  cout << "testBackendLoadingWithInvalidParameters" << (success ? "OK" : "FAIL") << endl;

  success = success && testBackendLoadingWithValidConnectionInfo("sqlite3");
  cout << "testBackendLoadingWithValidConnectionInfo" << (success ? "OK" : "FAIL") << endl;
 
#ifdef HAVE_LIBMYSQLCLIENT
  success = success && testPluginLoading(LIBNAME(litesql_backend_mysql), "param=b1");
  success = success && testBackendLoadingWithInvalidParameters("mysql");
#endif

#ifdef HAVE_LIBPQ
  success = success && testPluginLoading(LIBNAME(litesql_backend_pq), "param=b1");
#endif

  return success ? 0 : -1;
}


