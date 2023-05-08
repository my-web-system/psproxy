//
// Created by lucasg on 28/04/23.
//
#include <memory>
#include "../../includes/tcp/tcp-client.h"
#include "../../includes/debug.h"

TcpClient::TcpClient(int port, const std::string &host) : _port(port), _host(host) {
  // create socket
  _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket_fd < 0) {
	perror("psp client socket creation failed");
	exit(1);
  }
  _addr.sin_family = AF_INET;
  _addr.sin_port = htons(_port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, _host.c_str(), &_addr.sin_addr) <= 0) {
	perror("Error converting client IP");
	exit(1);
  }
}

void TcpClient::Start() {
  int status = connect(_socket_fd, (struct sockaddr *)&_addr, sizeof(_addr));
  if (status < 0) {
	perror("psp client socket connection failed");
	exit(1);
  } else {
	Debug::Print("psp connected to client " + _host + ":" + std::to_string(_port), Debug::Level::INFO);
  }
}

std::shared_ptr<Packet<HTTPResponse>> TcpClient::Forward(const std::shared_ptr<Packet<HTTPRequest>> &packet) {
  this->Send(_socket_fd, packet->getHTTPObject()->toBuffer().c_str(), packet->getHTTPObject()->toBuffer().length());
  std::string buffer = this->Read(_socket_fd);
  auto httpResponse = new HTTPResponse(buffer);
  std::shared_ptr<Packet<HTTPResponse>>
	  packetResponse(new Packet<HTTPResponse>(packet->getServerSocket(), httpResponse));
  return packetResponse;
}

TcpClient::~TcpClient() {
  Debug::Print("Close client " + std::to_string(_socket_fd), Debug::Level::INFO);
  shutdown(_socket_fd, SHUT_RDWR);
  close(_socket_fd);
}
