cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CUDA_ARCHITECTURES "native" CACHE STRING "CUDA architectures to target")

project(cuda_demo LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_executable(cuda_vector_add
    src/main.cpp
    src/cuda_vector_add.cu
)

target_include_directories(cuda_vector_add PRIVATE src)
