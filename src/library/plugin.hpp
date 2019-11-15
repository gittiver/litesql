#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "sharedlibrary.hpp"
namespace litesql {
  class Backend;

class Plugin {
  typedef Backend* (*fpCreateBackend)(const std::string& param);
  typedef void (*fpDeleteBackend)(litesql::Backend* backend);

  friend class PluginLoader;
public:
  virtual ~Plugin();

  static Plugin* load(const char* libname);
  Backend* create(const std::string& parameter);
  void destroy(Backend* pBackend);
protected:
  Plugin(SharedLibrary* sharedLib,
         void* fCreate,
         void* fDestroy );
private:

  void* m_fCreate = nullptr;
  void* m_fDestroy = nullptr;
  SharedLibrary* m_sharedLib = nullptr;
};
}
#endif // #ifndef PLUGIN_HPP

