//
// Created by lucasg on 27/04/23.
//

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <memory>
#include "../../includes/tcp/tcp-server.h"
#include "../../includes/debug.h"

TcpServer::TcpServer(int port) {
  _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket_fd < 0) {
	perror("psp server socket creation failed");
	exit(1);
  }
  _addr.sin_family = AF_INET;
  _addr.sin_addr.s_addr = INADDR_ANY;
  _addr.sin_port = htons(port);
  if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
	perror("psp server socket configuration failed");
	exit(1);
  }
  Debug::Print("PSP TCP Server started.", Debug::Level::INFO);
}

TcpServer::~TcpServer() {}

void TcpServer::Listen() {
  if (listen(_socket_fd, 100) < 0) {
	perror("psp server socket listen failed");
	exit(1);
  }
}

void TcpServer::Bind() {
  if (bind(_socket_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
	perror("psp server socket bind failed");
	exit(1);
  }
}

// Shared ptr here cause it may be used in another thread
std::shared_ptr<Packet<HTTPRequest>> TcpServer::Accept() {
  int addr_len = sizeof(sockaddr_in);
  int com_socket = 1;
  std::string buffer(2048, 0);
  std::string generic_resp = {};

  com_socket = accept(_socket_fd, (struct sockaddr *)&_addr, (socklen_t *)&addr_len);
  if (com_socket < 0) {
	return nullptr;
  }

  read(com_socket, &buffer[0], 2048);
  // PSP_DBG(buffer.c_str())

  auto httpRequest = new HTTPRequest(buffer);
  std::shared_ptr<Packet<HTTPRequest>> packet(new Packet<HTTPRequest>(com_socket, httpRequest));

  return packet;
}

void TcpServer::Close() {
  shutdown(_socket_fd, SHUT_RDWR);
  close(_socket_fd);
}
