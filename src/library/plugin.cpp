#include <iostream>
#include "plugin.hpp"
#include "sharedlibrary.hpp"
#include "litesql/backend.hpp"

using namespace std;
using namespace litesql;

Plugin::Plugin( SharedLibrary* sharedLib,
                void* fCreate,
                void* fDestroy )
: m_fCreate(fCreate)
, m_fDestroy(fDestroy)
, m_sharedLib(sharedLib)
{}
Plugin::~Plugin()
{
  delete m_sharedLib;
}

Plugin* Plugin::load(const char* libname)
{
  Plugin* pPlugin=nullptr;
  std::string error;
  auto lib = SharedLibrary::load(libname,&error);
  if (!lib) {
    cerr  << error  << endl;
  } else {
    auto createFunction = lib->loadSymbol("createBackend",&error);
    if (!createFunction) {
      delete lib;
      cerr << error << endl;
    } else {
      auto destroyFunction = lib->loadSymbol("deleteBackend",&error);
      if (!destroyFunction) {
        delete lib;
        cerr << error << endl;
      } else {
        pPlugin = new Plugin(lib,createFunction,destroyFunction);
      }
    }
  }
  return pPlugin;
}

Backend*  Plugin::create(const std::string& parameter) {
  return ((fpCreateBackend)m_fCreate)(parameter);
}

void  Plugin::destroy(Backend* pBackend) {
  if (pBackend)
    ((fpDeleteBackend)m_fDestroy)(pBackend);
}
