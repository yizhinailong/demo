cmake_minimum_required(VERSION 4.1.0)
project(stdexec_example)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "" FORCE)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME stdexec
    GITHUB_REPOSITORY NVIDIA/stdexec
    GIT_TAG main
)

function(add_stdexec_example name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE STDEXEC::stdexec)
endfunction()

add_stdexec_example(hello_world)
