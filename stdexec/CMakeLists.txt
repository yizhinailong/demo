cmake_minimum_required(VERSION 4.1.0)
project(stdexec_example)

include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "" FORCE)

CPMAddPackage(
    NAME stdexec
    GITHUB_REPOSITORY NVIDIA/stdexec
    GIT_TAG main
)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world PRIVATE STDEXEC::stdexec)
