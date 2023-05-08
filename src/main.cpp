#include "../includes/proxy.h"
#include "../includes/debug.h"

int main(int argc, char **argv) {
  // Setup and conf
  Config::init("../config/sample.yaml");

  std::unique_ptr<Proxy> proxy = std::make_unique<Proxy>(Config::getInstance()->getConfig());

  proxy->Start();

  return 0;
}