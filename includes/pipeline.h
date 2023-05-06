//
// Created by lucasg on 05/05/23.
//

#ifndef PSPROXY_PIPELINE_H
#define PSPROXY_PIPELINE_H
#include <vector>
#include <memory>

#include "plugin.h"

class Pipeline {
public:
    Pipeline();
    ~Pipeline();

    void Load();
    void Load(std::string path);

    void Run(std::string &message);

private:
    std::vector<std::shared_ptr<Plugin>> _pluginChain;
};

#endif //PSPROXY_PIPELINE_H
