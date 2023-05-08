//
// Created by lucasg on 05/05/23.
//
#include "../includes/pipeline.h"
#include "../includes/debug.h"
#include <dirent.h>
#include <iostream>

Pipeline::Pipeline() {
  this->Load();
}

Pipeline::~Pipeline() {

}

void Pipeline::Run(std::string &message) {
  for (auto &plugin : _pluginChain) {
	plugin->Run(message);
  }
}

void Pipeline::Load() {
  // Default load from /usr/local/lib/psproxy (mkdir at install time)
  // Use old school way (now std::filesystem exists, but you know...)
  DIR *dir;
  struct dirent *dirent;
  std::string path = "/usr/local/lib/psproxy/";
  std::vector<std::string> sharedLibs;

  if ((dir = opendir(path.c_str())) != nullptr) {
	while ((dirent = readdir(dir)) != nullptr) {
	  if (std::string(dirent->d_name).find(".so") != std::string::npos) {
		sharedLibs.emplace_back(path + dirent->d_name);
	  }
	}
	closedir(dir);
  } else {
	// error
  }

  // Now load plugins
  for (auto &lib : sharedLibs) {
	try {
	  auto plugin = std::make_shared<Plugin>(lib);
	  Debug::Print("Lib " + lib + " loaded.", Debug::Level::DBG);
	  _pluginChain.push_back(plugin);
	} catch (PluginLoadException &e) {
	  // error loading plugin
	  std::cout << e.what() << std::endl;
	}
  }
}

