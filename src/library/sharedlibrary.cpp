#include "sharedlibrary.hpp"
#include <iostream>
#include <dlfcn.h>

using namespace std;

SharedLibrary::SharedLibrary(void* handle)
: dlHandle(handle)
//, functions()
{
  cout << "loaded shared lib" << endl;
}

SharedLibrary::~SharedLibrary()
{
  if (dlHandle) {
    dlclose(dlHandle);
    dlHandle = nullptr;
    cout << "unloaded shared lib" << endl;
  }
}

shared_ptr<SharedLibrary> SharedLibrary::load(const char* libname)
{
  shared_ptr<SharedLibrary> library;

  void* dlHandle = dlopen(libname, RTLD_NOW);
  if (!dlHandle) {
    cerr << "opening dlib " << libname << endl;
  }
  else {
    library = shared_ptr<SharedLibrary>(new SharedLibrary(dlHandle));
  }
  return library;
}

std::string SharedLibrary::getError() {
  string e;
  const char* pErrorString = dlerror();
  if (pErrorString)
  {
    e = pErrorString;
  }
  return e;
}

shared_ptr<SharedLibrary::Symbol> SharedLibrary::loadSymbol(const char* symbolname)
{
  SharedLibrary::Symbol* symbol = nullptr;
  if (!dlHandle) {
    
  } else {
//    auto f = functions.find(symbolname);
//    if (f==functions.end()) {
      void *symHandle = dlsym(dlHandle, symbolname);
      if (!symHandle) {
        cerr << "symbol not found " << symbolname << endl;
      } else {
        symbol = new Symbol(this,symHandle,symbolname);
      }
    }
//  }
  return unique_ptr<Symbol>(symbol);
}

SharedLibrary::Symbol::Symbol(SharedLibrary* /*lib*/,void* symbolHandle,const string& name)
: //library(lib) ,
m_symbolHandle(symbolHandle)
, m_name(name)
{}

SharedLibrary::Symbol::~Symbol()
{
  //  library->functions.erase(m_name);
}

const std::string& SharedLibrary::Symbol::getName() const
{ return m_name; }

void* SharedLibrary::Symbol::getHandle()
{ return m_symbolHandle; }
