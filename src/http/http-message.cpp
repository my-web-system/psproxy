//
// Created by lucasg on 28/04/23.
//

#include <vector>
#include "../../includes/http/http-message.h"
#include <boost/beast.hpp>

HTTPRequest::HTTPRequest(std::string &buffer) : HTTPMessage() {
    boost::beast::error_code error;
    boost::beast::http::parser<true, boost::beast::http::string_body> parser;
    parser.put(boost::asio::buffer(buffer), error);

    // Holding major / minor
    auto condensedVersion = parser.get().version();
    unsigned major = condensedVersion / 10;
    unsigned minor = condensedVersion % 10;
    _version = std::to_string(major) + "." + std::to_string(minor);
    _method = to_string(parser.get().method());
    for (auto &header: parser.get().base()) {
        _headers.insert(std::pair<std::string, std::string>(header.name_string(), header.value()));
    }
    _body = parser.get().body();
    _uri = Uri(parser.get().target());
}

std::string HTTPRequest::toBuffer() {
    std::string crlf = "\r\n";
    return _method + " " + _uri.getProxyRedirectionUri() + " " + "HTTP/" + _version + crlf
           + this->getFlatHeaders()
           + crlf
           + _body;
}

HTTPResponse::HTTPResponse(std::string &buffer) : HTTPMessage() {
    boost::beast::error_code error;
    boost::beast::http::parser<false, boost::beast::http::string_body> parser;
    parser.eager(true); // force boost to parse all message, required to parse body
    parser.put(boost::asio::buffer(buffer), error);

    // Holding major / minor
    auto condensedVersion = parser.get().version();
    unsigned major = condensedVersion / 10;
    unsigned minor = condensedVersion % 10;
    _version = std::to_string(major) + "." + std::to_string(minor);
    for (auto &header: parser.get().base()) {
        _headers.insert(std::pair<std::string, std::string>(header.name_string(), header.value()));
    }
    _body = parser.get().body();
    _status = parser.get().result_int();
    _reason = parser.get().reason();
}

std::string HTTPResponse::toBuffer() {
    std::string crlf = "\r\n";
    return "HTTP/" + _version + " " + std::to_string(_status) + " " + _reason + crlf
           + this->getFlatHeaders()
           + crlf
           + _body;
}
