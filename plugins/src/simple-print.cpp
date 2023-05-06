//
// Created by lucasg on 05/05/23.
//
#include <string>
#include <iostream>

extern "C" void PSPEntry(std::string message)
{
    std::cout << "I'm the simple print plugin" << std::endl;
    std::cout << message << std::endl;
}