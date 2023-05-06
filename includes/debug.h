//
// Created by lucasg on 27/04/23.
//

#ifndef PSPROXY_DEBUG_H
#define PSPROXY_DEBUG_H

#include <iostream>

#define PSP_PRINT(msg, lvl) std::cout << "[" << lvl << "] " << msg << std::endl;

#define PSP_DBG(msg) PSP_PRINT(msg, "DEBUG")
#define PSP_INFO(msg) PSP_PRINT(msg, "INFO")
#define PSP_WARN(msg) PSP_PRINT(msg, "WARN")
#define PSP_ERR(msg) PSP_PRINT(msg, "ERR")

#endif //PSPROXY_DEBUG_H
