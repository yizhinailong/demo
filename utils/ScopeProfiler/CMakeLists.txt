cmake_minimum_required(VERSION 4.0)
project(ScopeProfiler)

set(CMAKE_CXX_STANDARD 26)

add_executable(main src/main.cpp)
target_include_directories(main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
