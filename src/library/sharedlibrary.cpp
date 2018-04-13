#include "sharedlibrary.hpp"
#include <iostream>
#include <dlfcn.h>

using namespace std;

SharedLibrary::SharedLibrary(void* handle)
: dlHandle(handle)
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

SharedLibrary* SharedLibrary::load(const char* libname, std::string* pError)
{
  SharedLibrary* library = nullptr;

  void* dlHandle = dlopen(libname, RTLD_NOW);
  if (!dlHandle) {
    if (pError) {
      *pError = dlerror();
    }
  }
  else {
    library = new SharedLibrary(dlHandle);
  }
  return library;
}

void* SharedLibrary::loadSymbol(const char* symbolname, std::string* pError)
{
  void* symbol = nullptr;
  if (!dlHandle) {
    if (pError)
    {
      *pError = "no dlHandle";
    }
  } else {
      symbol = dlsym(dlHandle, symbolname);
      if (!symbol) {
        if (pError) {
          *pError = dlerror();
        }
      }
    }
  return symbol;
}
