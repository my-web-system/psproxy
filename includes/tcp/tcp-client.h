//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_TCP_CLIENT_H
#define PSPROXY_TCP_CLIENT_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include "../http/packet.h"
#include "tcp.h"

class TcpClient: public Tcp {
public:
    TcpClient(int port, const std::string &host);
    ~TcpClient();

    // Bind, listen and run
    void Start();

    // PSP to server
    std::shared_ptr<Packet<HTTPResponse>> Forward(const std::shared_ptr<Packet<HTTPRequest>> &packet);

private:
    int _socket_fd;
    sockaddr_in _addr;
    int _port;
    std::string _host;
};

#endif //PSPROXY_TCP_CLIENT_H
