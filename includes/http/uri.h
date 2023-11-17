#ifndef PSPROXY_URI_H
#define PSPROXY_URI_H

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

#define STR_BACKSL_TOKEN "/"
#define CHAR_BACKSL_TOKEN '/'

class UriException : public std::exception {
public:
    UriException(const std::string &uri): _uri(uri);

    virtual const char *what() const noexcept override {
        return std::string("Failed to resolve uri: " + _uri).c_str();
    }

private:
    std::string _uri;
};

class Uri {
public:
    Uri(const std::string &uri);
    Uri() = default;

    void setUri(const std::string &uri);
    std::string getUri();

    void setProxyRedirectionUri(const std::string &uri);
    std::string getProxyRedirectionUri();

    void buildProxyRedirectionUri(const std::string &server);

private:
    void clearUriTrailingBackslashes();

    bool checkUriPath();

    friend std::ostream &operator<<(std::ostream &os, const Uri &uri);

private:
    std::string _uri;
    std::string _proxyRedirectionUri;
};

#endif //PSPROXY_URI_H
