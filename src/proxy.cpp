//
// Created by lucasg on 28/04/23.
//

#include <iostream>
#include <unistd.h>
#include <csignal>
#include "../includes/proxy.h"
#include "../includes/debug.h"

volatile std::atomic_bool Proxy::_is_terminated{false};

Proxy::Proxy(const PSProxyConfig &config) : _config(config), _upward_pipeline(Pipeline()) {
  _server = std::make_unique<TcpServer>(config.port);

  SetupClientPool();

  // TODO catch for SEGV etc... to gracefull shutdown
  std::signal(SIGINT | SIGTERM, Proxy::terminationSignalHandler);
}

Proxy::~Proxy() {
}

void Proxy::Start() {
  _server->Bind();
  _server->Listen();

  // Setup tcp client upward daemon
  std::thread clientDaemon1(&Proxy::upward_daemon, this);
  clientDaemon1.detach();
  // Setup tcp downward daemon
  std::thread downwardDaemon(&Proxy::downward_daemon, this);
  downwardDaemon.detach();

  // Setup server termination
  std::thread serverTermination([this]() {
	while (!Proxy::_is_terminated) {
	  std::this_thread::yield();
	}
	this->_server->Close();
  });
  serverTermination.detach();

  while (!Proxy::_is_terminated) {
	std::shared_ptr<Packet<HTTPRequest>> packet = _server->Accept();
	if (!packet) {
	  // most likely end of program catch, or accept socket error
	  if (!Proxy::_is_terminated) {
		Debug::Print("Could not process packet, moving to next one", Debug::Level::WARN);
	  }
	  continue;
	}
	try {
	  // Resolves host/port through config (using HTTPRequest _uri)
	  packet->resolve();
	}  catch (const UriException &e) {
        Debug::Print(e.what(), Debug::Level::ERR);
        // downward HTTP exception

    } catch (const std::runtime_error &e) {
	  Debug::Print(e.what(), Debug::Level::ERR);
	  continue;
	} catch (const std::exception &e) {
	  Debug::Print(e.what(), Debug::Level::ERR);
	  continue;
	}
	std::lock_guard<std::mutex> guard(_g_upward_queue_mutex);
	_upward_queue.push(std::move(packet));
  }
  Debug::Print("PSProxy says goodbye :)", Debug::Level::INFO);
}

void Proxy::upward_daemon() {
  while (!Proxy::_is_terminated) {
	std::lock_guard<std::mutex> guard(_g_upward_queue_mutex);
	if (!_upward_queue.empty()) {
	  auto packet = _upward_queue.front();
	  auto buffer = packet->getHTTPObject()->toBuffer();

	  // Send to upward pipeline
	  _upward_pipeline.Run(buffer);

	  this->forward(packet);
	  _upward_queue.pop();
	}
  }
  Debug::Print("Upward daemon terminated.", Debug::Level::INFO);
}

void Proxy::forward(std::shared_ptr<Packet<HTTPRequest>> &packet) {
  auto client = _clientPool.at(packet->geTarget().name);
  auto packetResponse = client->Forward(packet);
  _downward_queue.push(std::move(packetResponse));
}

void Proxy::downward_daemon() {
  while (!Proxy::_is_terminated) {
	std::lock_guard<std::mutex> guard(_g_downward_queue_mutex);
	if (!_downward_queue.empty()) {
	  auto packet = _downward_queue.front();
	  // Send to downward pipeline
	  this->downward(packet);
	  _downward_queue.pop();
	}
  }
  Debug::Print("Downward daemon terminated.", Debug::Level::INFO);
}

void Proxy::downward(std::shared_ptr<Packet<HTTPResponse>> &packet) {
  std::cout << packet->getHTTPObject()->toBuffer() << std::endl;
  send(packet->getServerSocket(),
	   packet->getHTTPObject()->toBuffer().c_str(),
	   packet->getHTTPObject()->toBuffer().length(),
	   0);
  close(packet->getServerSocket());
}

void Proxy::terminationSignalHandler(int signum) {
  Debug::Print("Terminating from signal.", Debug::Level::INFO);
  _is_terminated = true;
}

void Proxy::SetupClientPool() {
  for (const auto &server : _config.servers) {
	std::pair<std::string, std::shared_ptr<TcpClient>> client;

	client.first = server.name;
	client.second = std::make_shared<TcpClient>(server.port, server.host);
	// Connect
	client.second->Start();
	_clientPool.insert(client);
  }
}
