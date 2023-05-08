//
// Created by lucasg on 30/04/23.
//

#ifndef PSPROXY_CONFIG_PARSER_H
#define PSPROXY_CONFIG_PARSER_H

#include <vector>
#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iostream>

struct LogConfig {
  std::string level;
};

struct ServerConfig {
  std::string name;
  std::string scheme;
  std::string host;
  int port;
};

struct PSProxyConfig {
  LogConfig log;
  int port;
  std::vector<ServerConfig> servers;
};

class ConfigParser {
 public:
  explicit ConfigParser(const std::string &configPath);

  ~ConfigParser() = default;

  PSProxyConfig ParseConfigFile();

  void PrintConfig(const PSProxyConfig &config);

 private:
  std::string _configPath;
};

#endif //PSPROXY_CONFIG_PARSER_H
