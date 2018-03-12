#ifndef SHAREDLIBRARY_HPP
#define SHAREDLIBRARY_HPP

#include <map>
#include <string>

/** contains the ability to load symbols from shared Libraries on *NIX and OSX
 * or dynamic link Libraries on windows.
 * The loaded symbols cann be used to call functions or access data
 * of the loaded libraries.
 * @TODO implement windows dynamic linking functionality
 */
class SharedLibrary {
public:
  virtual ~SharedLibrary();

  class Symbol {
  public:
    Symbol(SharedLibrary* lib, void* symbolHandle,const std::string& name);
    virtual ~Symbol();
    const std::string& getName() const;
    void* getHandle();
  private:
//    SharedLibrary* library;
    void* m_symbolHandle;
    std::string m_name;
  };

  /** tries to load a shared library from a file with filename.
   * if successful, the sharedLibrary is returned, on error a nullptr Pointer is returned
   * The error reason can be retrieved by method getError().
   * @param filename name of the file to load
   */
  static std::shared_ptr<SharedLibrary> load(const char* filename);

  /** returns a string containing the cause of the last error.   */
  static std::string getError();

  /** load an (exported) symbol from the shared Library.
   * if successful, the Symbol is returned, on error a nullptr Pointer is returned.
   * The error reason can be retrieved by method getError().
   *
   * For using the symbol call its getHandle() Method and use the retrieved handle
   * together with a cast into a function pointer or variable adress.
   * @param symbolname name of the symbol to load
   */
  std::shared_ptr<Symbol> loadSymbol(const char* symbolname);

private:
  SharedLibrary(void* handle);

  void* dlHandle;
//  std::map<std::string,std::shared_ptr<Symbol> > functions;
};
#endif //#ifndef SHAREDLIBRARY_HPP
