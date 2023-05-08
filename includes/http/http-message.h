//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_HTTP_MESSAGE_H
#define PSPROXY_HTTP_MESSAGE_H

#include <string>
#include <map>

typedef std::map<std::string, std::string> HTTPHeaders;

class HTTPMessage {
 public:
  HTTPMessage() = default;
  ~HTTPMessage() = default;

  virtual std::string toBuffer() = 0;

 protected:
  inline std::string getVersion() { return _version; };
  inline std::string getBody() { return _body; };
  inline HTTPHeaders getHeaders() { return _headers; };

  std::string getFlatHeaders() {
	std::string headers;

	if (_headers.empty()) {
	  return headers;
	}
	for (auto &pair : _headers) {
	  headers.append(pair.first + ": " + pair.second + "\r\n");
	}
	return headers;
  }

 protected:
  std::string _version;
  std::string _body;
  HTTPHeaders _headers;
};

class HTTPRequest : public HTTPMessage {
 public:
  HTTPRequest(std::string &buffer);
  ~HTTPRequest() = default;

  std::string toBuffer() override;

  std::string _method;
  std::string _uri;
};

class HTTPResponse : public HTTPMessage {
 public:
  HTTPResponse(std::string &buffer);
  ~HTTPResponse() = default;

  std::string toBuffer() override;

  unsigned int _status;
  std::string _reason;
};

#endif //PSPROXY_HTTP_MESSAGE_H
