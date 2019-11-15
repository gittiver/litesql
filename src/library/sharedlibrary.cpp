#include "sharedlibrary.hpp"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

SharedLibrary::SharedLibrary(HSHAREDLIBRARY handle)
: dlHandle(handle)
{
  cout << "loaded shared lib" << endl;
}

SharedLibrary::~SharedLibrary()
{
  if (dlHandle) {
#ifdef WIN32
	  FreeLibrary(dlHandle);
#else
	  dlclose(dlHandle);
#endif //#ifdef WIN32
    dlHandle = nullptr;
    cout << "unloaded shared lib" << endl;
  }
}

SharedLibrary* SharedLibrary::load(const char* libname, std::string* pError)
{
  SharedLibrary* library = nullptr;
  HSHAREDLIBRARY dlHandle;
#ifdef WIN32
  dlHandle = LoadLibrary(libname);
  if (!dlHandle) {
	  if (pError) {
		  *pError = "could not open DLL: ";
		  *pError += libname;
	  }
  }
#else
  dlHandle = dlopen(libname, RTLD_NOW);
  if (!dlHandle) {
	  if (pError) {
		  *pError = dlerror();
	  }
  }
#endif //#ifdef WIN32
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
#ifdef WIN32
	  symbol = GetProcAddress(dlHandle, symbolname);
	  if (!symbol) {
		  if (pError) {
			  *pError = "symbol not found:";
			  *pError += symbolname;
		  }
	  }
#else
	  symbol = dlsym(dlHandle, symbolname);
	  if (!symbol) {
		  if (pError) {
			  *pError = dlerror();
		  }
	  }
#endif //#ifdef WIN32
    }
  return symbol;
}
