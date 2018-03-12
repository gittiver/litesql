/* LiteSQL - test-backend-loading
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include "litesql/database.hpp"

/*
*/
using namespace litesql;

int main(int /*argc*/, char * /*argv*/ []) {

  bool success = false;
  try {
    Database sqlite3_db("sqlite3","database=test-update-table.db");
    success = false;

#ifdef HAVE_LIBMYSQLCLIENT
    Database mysql_db("mysql","database=test-update-table.db");
    success &= Updater::testUpgradeTable(mysql_db);
#endif

#ifdef HAVE_LIBPQ
    Database pg_db("postgresql","host=localhost;database=test-update-table;user=litesql;password=litesql");
    success &= Updater::testUpgradeTable(pg_db);
#endif

#ifdef HAVE_OCILIB
    Database oracle_db("oracle","database=test-update-table.db");
    success &= Updater::testUpgradeTable(oracle_db);
#endif

#ifdef HAVE_ODBC
    Database odbc_db("odbc","database=test-update-table.db");
    success &= Updater::testUpgradeTable(odbc_db);
#endif

  } catch (Except e) {
      cerr << e << endl;
      return -1;
  }
    return success ? 0 : -1;
}


