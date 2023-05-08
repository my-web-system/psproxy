#include "../../includes/config/config-parser.h"

void operator>>(const YAML::Node &node, LogConfig &logConfig) {
  logConfig.level = node["level"].Scalar();
}

void operator>>(const YAML::Node &node, ServerConfig &serverConfig) {
  serverConfig.name = node["name"].Scalar();
  serverConfig.scheme = node["scheme"].Scalar();
  serverConfig.host = node["host"].Scalar();
  serverConfig.port = std::atoi(node["port"].Scalar().c_str());
}

ConfigParser::ConfigParser(const std::string &configPath) : _configPath(configPath) {}

PSProxyConfig ConfigParser::ParseConfigFile() {
  PSProxyConfig config;
  try {
	YAML::Node doc = YAML::LoadFile(_configPath);
	LogConfig logConfig;
	std::vector<ServerConfig> serversConfig;

	doc["log"] >> logConfig;
	config.port = std::atoi(doc["port"].Scalar().c_str());
	auto servers = doc["servers"];
	for (const auto &server : servers) {
	  ServerConfig serverConfig;
	  server >> serverConfig;
	  serversConfig.push_back(serverConfig);
	}
	config.log = logConfig;
	config.servers = serversConfig;
  } catch (YAML::ParserException &e) {
	std::cout << e.what() << std::endl;
  }
  // if psp log level == DBG then print config
  return config;
}

void ConfigParser::PrintConfig(const PSProxyConfig &config) {
  std::cout << "--------" << std::endl;
  std::cout << "PSProxy loaded configuration:" << std::endl;
  std::cout << "LOG_LVL: " << config.log.level << std::endl;
  std::cout << "PSP_PORT: " << config.port << std::endl;
  std::cout << config.servers.size() << " servers loaded:" << std::endl;
  for (const auto &server : config.servers) {
	std::cout << "\t-name: " << server.name << std::endl;
	std::cout << "\t scheme: " << server.scheme << std::endl;
	std::cout << "\t host: " << server.host << std::endl;
	std::cout << "\t port: " << server.port << std::endl;
  }
  std::cout << "--------" << std::endl;
}