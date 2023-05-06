//
// Created by lucasg on 06/05/23.
//

#ifndef PSPROXY_TCP_H
#define PSPROXY_TCP_H
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

constexpr int tcp_buffer_size = 1024;

class Tcp {
public:
    Tcp() = default;
    ~Tcp() = default;

protected:
    void Send(int fd, const void *buffer, int size);
    std::string &Read(int fd);
};

#endif //PSPROXY_TCP_H
