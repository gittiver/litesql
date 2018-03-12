#include <iostream>
#include "plugin.hpp"
#include "sharedlibrary.hpp"
#include "litesql/backend.hpp"

using namespace std;

Plugin::Plugin( std::shared_ptr<SharedLibrary> sharedLib,
                std::shared_ptr<SharedLibrary::Symbol> fCreate,
                std::shared_ptr<SharedLibrary::Symbol> fDestroy )
: m_sharedLib(sharedLib)
, m_fCreate(fCreate)
, m_fDestroy(fDestroy)
{}
               
unique_ptr<Plugin> Plugin::load(const char* libname)
{
  Plugin* pPlugin=nullptr;

  auto lib = SharedLibrary::load(libname);
  if (!lib) {
    cerr  << SharedLibrary::getError()  << endl;
  } else {
    auto createFunction = lib->loadSymbol("createBackend");
    if (!createFunction) {
      cerr << lib->getError() << endl;
    } else {
      auto destroyFunction = lib->loadSymbol("deleteBackend");
      if (!destroyFunction) {
        cerr << lib->getError() << endl;
      } else {
        pPlugin = new Plugin(lib,createFunction,destroyFunction);
      }
    }
  }
  return unique_ptr<Plugin>(pPlugin);
}

Backend*  Plugin::create(const std::string& parameter) {
  return ((fpCreateBackend)m_fCreate->getHandle())(parameter);
}

void  Plugin::destroy(Backend* pBackend) {
  ((fpDeleteBackend)m_fDestroy->getHandle())(pBackend);
}
