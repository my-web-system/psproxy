//
// Created by lucasg on 28/04/23.
//

#ifndef PSPROXY_IOINTERCEPTOR_H
#define PSPROXY_IOINTERCEPTOR_H

#include "../http/http-message.h"

class IOInterceptorInterface {
public:
    IOInterceptorInterface();
    virtual ~IOInterceptorInterface() = 0;

    virtual void InInterceptor(HTTPMessage http_msg) = 0;
    virtual void OutInterceptor(HTTPMessage http_msg) = 0;
};

#endif //PSPROXY_IOINTERCEPTOR_H
