//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_PLUGIN_H
#define PSPROXY_PLUGIN_H

#define PSP_EXTERN_SYMBOL "PSPEntry"

#include <string>

typedef void (*PSPExternPluginEntry)(std::string);

class PluginLoadException : public std::exception {
 public:
  PluginLoadException(const std::string &plugin) : _plugin(plugin) {}

  virtual const char *what() const noexcept override {
	return std::string("Failed to load plugin: " + _plugin).c_str();
  }

 private:
  std::string _plugin;
};

class Plugin {
 public:
  explicit Plugin(const std::string &dllPath);
  ~Plugin();

  void Run(std::string &message);

 private:
  // DLL extern function
  PSPExternPluginEntry _entry;
  void *_dll;
};

#endif //PSPROXY_PLUGIN_H
