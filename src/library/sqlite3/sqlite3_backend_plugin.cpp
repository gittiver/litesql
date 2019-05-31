#include "litesql/backend.hpp"
#include "sqlite3.hpp"

using namespace litesql;

extern "C" Backend* createBackend(const std::string& parameter)
{
  Backend* pBackend = nullptr;
  try {
    pBackend = new SQLite3(parameter);
  } catch(const DatabaseError& /*ex*/) {
    pBackend = nullptr;
  }
  return pBackend;
}

extern "C" void deleteBackend(Backend* backend)
{
  if (backend)
  {
    delete backend;
  }
}
