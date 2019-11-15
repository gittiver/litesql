#ifndef SHAREDLIBRARY_HPP
#define SHAREDLIBRARY_HPP

#include <string>

#ifdef WIN32
#include <Windows.h>
#define HSHAREDLIBRARY HMODULE
#else
#include <dlfcn.h>
#define HSHAREDLIBRARY void*
#endif //#ifdef WIN32

/** contains the ability to load symbols from shared Libraries on *NIX and OSX
 * or dynamic link Libraries on windows.
 * The loaded symbols can be used to call functions or access data
 * of the loaded libraries.
 * @TODO implement windows dynamic linking functionality
 */
class SharedLibrary {
public:
  virtual ~SharedLibrary();

  /** tries to load a shared library from a file with filename.
   * if successful, the sharedLibrary is returned, on error a nullptr Pointer is returned
   * The error reason can be retrieved by method getError().
   * @param filename name of the file to load
   */
  static SharedLibrary* load(const char* filename, std::string* pError);

  /** load an (exported) symbol from the shared Library.
   * if successful, the Symbol is returned, on error a nullptr Pointer is returned.
   * The error reason can be retrieved by method getError().
   *
   * For using the symbol call its getHandle() Method and use the retrieved handle
   * together with a cast into a function pointer or variable adress.
   * @param symbolname name of the symbol to load
   */
  void* loadSymbol(const char* symbolname, std::string* pError);

private:
  SharedLibrary(HSHAREDLIBRARY handle);

  HSHAREDLIBRARY dlHandle;
};
#endif //#ifndef SHAREDLIBRARY_HPP
