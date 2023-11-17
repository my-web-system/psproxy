//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_DEBUG_H
#define PSPROXY_DEBUG_H

#include <iostream>
#include <sstream>
#include <iomanip>

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

class Debug {
 public:
  Debug() = default;
  ~Debug() = default;

  Debug(const Debug &) = delete;
  void operator=(const Debug &) = delete;

  enum Level {
	INFO = 0,
	DBG = 1,
	WARN = 2,
	ERR = 3
  };

  static std::string currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
  }

  static void Print(const std::string &message, Level level) {
	auto time = std::time(nullptr);
	auto tm = *std::localtime(&time);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%YT%H:%M:%S");
	auto timestamp = oss.str();

	output(message, timestamp, levelToString(level));
  }

 private:

  static void output(const std::string &message, std::string &timestamp, std::string level) {
	std::cout << "[" << level << "] [" << timestamp << "] " << message << std::endl;
  }

  static std::string levelToString(Level level) {
	switch (level) {
	  case INFO: return FWHT("INFO");
	  case DBG: return FMAG("DEBUG");
	  case WARN: return FYEL("WARN");
	  case ERR: return FRED("ERR");
	}
	throw std::runtime_error("Unknown log level");
  }
};

#endif //PSPROXY_DEBUG_H
