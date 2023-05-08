//
// Created by lucasg on 06/05/23.
//
#include "../../includes/tcp/tcp.h"
#include "../../includes/debug.h"

void Tcp::InitializeSSL() {
}

void Tcp::Send(int fd, const void *buffer, int size) {
  int bytes = send(fd, buffer, (ssize_t)size, 0);
  if (bytes < 0) {
	perror("TCP error on send");
  }
}

std::string &Tcp::Read(int fd) {
  std::string buffer(tcp_buffer_size, 0);
  std::string value;
  auto bytes = (ssize_t)buffer.size();

  while (bytes >= buffer.size()) {
	bytes = read(fd, &buffer[0], buffer.size());
	value = value.append(buffer, 0, bytes);
  }
  return *(new std::string(value));
}

