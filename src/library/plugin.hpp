#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "sharedlibrary.hpp"
class Backend;

class Plugin {
  typedef Backend* (*fpCreateBackend)(const std::string& param);
  typedef void (*fpDeleteBackend)(Backend* backend);

  friend class PluginLoader;
public:
  static std::unique_ptr<Plugin> load(const char* libname);
  Backend* create(const std::string& parameter);
  void destroy(Backend* pBackend);
protected:
  Plugin(std::shared_ptr<SharedLibrary> sharedLib,
         std::shared_ptr<SharedLibrary::Symbol> fCreate,
         std::shared_ptr<SharedLibrary::Symbol> fDestroy );
private:

  std::shared_ptr<SharedLibrary> m_sharedLib;
  std::shared_ptr<SharedLibrary::Symbol> m_fCreate;
  std::shared_ptr<SharedLibrary::Symbol> m_fDestroy;
};
#endif // #ifndef PLUGIN_HPP

