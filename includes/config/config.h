//
// Created by lucasg on 30/04/23.
//

#ifndef PSPROXY_CONFIG_H
#define PSPROXY_CONFIG_H

#include "config-parser.h"

class Config {
public:
    explicit Config(const std::string &configPath) : _parser(ConfigParser(configPath)) {
        _config = _parser.ParseConfigFile();
    }

    Config(const Config&) = delete;
    void operator=(const Config&) = delete;

    ~Config() {
        delete _instance;
    }

    static Config *getInstance() {
        return _instance;
    }

    static void init(const std::string &configPath) {
        if (!_instance) {
            _instance = new Config(configPath);
        }
    }

    const PSProxyConfig &getConfig() { return _config; };

private:
    ConfigParser _parser;
    PSProxyConfig _config;

    static Config *_instance;
};

#endif //PSPROXY_CONFIG_H
