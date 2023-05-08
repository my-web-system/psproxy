//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_PROXY_H
#define PSPROXY_PROXY_H

#include "tcp/tcp-client.h"
#include "tcp/tcp-server.h"
#include "http/packet.h"
#include "config/config.h"
#include "pipeline.h"

#include <cstdlib>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>

typedef std::map<std::string, std::shared_ptr<TcpClient>> TCPClientPool;

class Proxy {
 public:
  Proxy(const PSProxyConfig &config);
  ~Proxy();

  Proxy(const Proxy &) = delete;
  Proxy &operator=(Proxy &) = delete;

  void SetupClientPool();
  void Start();

 private:
  void upward_daemon();
  void downward_daemon();

  void forward(std::shared_ptr<Packet<HTTPRequest>> &packet);
  void downward(std::shared_ptr<Packet<HTTPResponse>> &packet);

  static void terminationSignalHandler(int signum);

 private:
  PSProxyConfig _config;

  std::unique_ptr<TcpServer> _server;
  TCPClientPool _clientPool;

  std::queue<std::shared_ptr<Packet<HTTPRequest>>> _upward_queue;
  std::queue<std::shared_ptr<Packet<HTTPResponse>>> _downward_queue;

  Pipeline _upward_pipeline;

  std::mutex _g_upward_queue_mutex;
  std::mutex _g_downward_queue_mutex;

  // Termination flag
  static volatile std::atomic_bool _is_terminated;
};

#endif //PSPROXY_PROXY_H
