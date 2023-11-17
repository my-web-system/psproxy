cmake_minimum_required(VERSION 3.24)
project(psproxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Load libraries
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)


# Boost is installed locally

# YAML parser (static link)
add_library(libyaml STATIC IMPORTED)
set_target_properties(libyaml PROPERTIES IMPORTED_LOCATION /Users/lucas/Documents/GitHub/psproxy/lib/libyaml/libyaml-cpp.a)
set_target_properties(libyaml PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /Users/lucas/Documents/GitHub/psproxy/lib/libyaml/include)

# Load source files
file(GLOB_RECURSE PSPROXY_SOURCES "src/*.cpp")
file(GLOB_RECURSE PSPROXY_HEADERS "includes/*.h")
file(GLOB_RECURSE TST_SOURCES "tests/*.cpp")
file(GLOB_RECURSE UT_SOURCES
        "src/http/uri.cpp"
)
file(GLOB_RECURSE UT_HEADERS
        "includes/http/uri.h"
)
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

# Unit tests
enable_testing()
add_executable(
        unitTests
        ${TST_SOURCES}
        ${UT_SOURCES}
        ${UT_HEADERS}
)
target_link_libraries(
        unitTests
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(unitTests)

# Plugins
ADD_LIBRARY(simple_print SHARED ${PSPROXY_PLUGINS_SIMPLE_PRINT})