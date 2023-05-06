//
// Created by lucasg on 28/04/23.
//

#ifndef PSPROXY_PACKET_H
#define PSPROXY_PACKET_H

#include <string>
#include "http-message.h"
#include "../config/config.h"
#include <boost/algorithm/string/replace.hpp>

template <typename T>
class Packet {
public:
    Packet(int socket_fd, T *http) {
        _socket_fd = socket_fd;
        _http = http;
    }
    ~Packet() = default;

    Packet(Packet &packet) {
        packet._socket_fd = _socket_fd;
        packet._http = _http;
    }

    template<typename = typename std::enable_if<std::is_same_v<T, HTTPRequest>>>
    void resolve() {
        auto config = Config::getInstance()->getConfig();
        auto request = static_cast<HTTPRequest*>(this->_http);

        for (const auto &server : config.servers) {
            if (request->_uri.find(server.name) != std::string::npos) {
                // resolver match
                _target = server;

                // Potentially modify packet _uri
                // GET /bpc/hello
                // become: GET /hello (psp redirects to /bpc server configured in yaml)
                std::string token = "/" + server.name;
                boost::replace_first(request->_uri, token, "");
                return;
            }
        }
        throw std::runtime_error("Can not resolve packet");
    }

    inline T *getHTTPObject() { return _http; }

    inline int getServerSocket() { return _socket_fd; }
    inline ServerConfig geTarget() { return _target; }

private:
    int _socket_fd;
    T *_http;
    ServerConfig _target;
};


#endif //PSPROXY_PACKET_H
