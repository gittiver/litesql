#include "litesql/backend.hpp"
#include "sqlite3.hpp"

using namespace litesql;

extern "C" Backend* createBackend(const std::string& parameter)
{
  return new SQLite3(parameter);
}

extern "C" void deleteBackend(Backend* backend)
{
  delete backend;
}
