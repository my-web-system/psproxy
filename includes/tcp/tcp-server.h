//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_TCP_SERVER_H
#define PSPROXY_TCP_SERVER_H

#include <netinet/in.h>
#include <memory>
#include "../http/packet.h"
#include "tcp.h"

class TcpServer: public Tcp {
public:
    explicit TcpServer(int port);
    ~TcpServer();

    void Bind();
    void Listen();
    std::shared_ptr<Packet<HTTPRequest>> Accept();
    void Close();

private:
    int _socket_fd;
    sockaddr_in _addr;

    int _opt = 1;

};

#endif //PSPROXY_TCP_SERVER_H
