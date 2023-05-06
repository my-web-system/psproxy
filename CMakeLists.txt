cmake_minimum_required(VERSION 3.24)
project(psproxy)

set(CMAKE_CXX_STANDARD 17)

# Load libraries
find_package(Threads REQUIRED)


# Boost is installed locally

# YAML parser (static link)
add_library(libyaml STATIC IMPORTED)
set_target_properties(libyaml PROPERTIES IMPORTED_LOCATION /usr/local/lib/libyaml-cpp.a)
set_target_properties(libyaml PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/yaml-cpp)

# Load source files
file(GLOB_RECURSE PSPROXY_SOURCES "src/*.cpp")
file(GLOB_RECURSE PSPROXY_HEADERS "includes/*.h")
set(PSPROXY_PLUGINS_SIMPLE_PRINT "plugins/src/simple-print.cpp")


# PSPROXY
add_executable(psproxy
        ${PSPROXY_SOURCES}
        ${PSPROXY_HEADERS}
)

TARGET_LINK_LIBRARIES(psproxy
        Threads::Threads
        libyaml
        ${CMAKE_DL_LIBS}
)

# Plugins
ADD_LIBRARY(simple_print SHARED ${PSPROXY_PLUGINS_SIMPLE_PRINT})