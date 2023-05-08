//
// Created by lucasg on 05/05/23.
//
#include "../includes/plugin.h"
#include <dlfcn.h>

Plugin::Plugin(const std::string &dllPath) {
  _dll = dlopen(dllPath.c_str(), RTLD_NOW);
  if (_dll == nullptr) {
	throw PluginLoadException(dllPath);
  }
  _entry = (PSPExternPluginEntry)dlsym(_dll, PSP_EXTERN_SYMBOL);
  if (_entry == nullptr) {
	throw PluginLoadException(dllPath);
  }
}

Plugin::~Plugin() {
  dlclose(_dll);
}

void Plugin::Run(std::string &message) {
  _entry(message);
}
