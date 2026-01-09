cmake_minimum_required(VERSION 3.10)
project(spdlog_demo)

macro(create_spdlog_app NAME)
    add_executable(${NAME} ${NAME}.cpp)
    target_link_libraries(${NAME} PRIVATE spdlog::spdlog)
endmacro()

# find_package(spdlog CONFIG REQUIRED)

# include(FetchContent)
# FetchContent_Declare(
#     spdlog
#     GIT_REPOSITORY https://github.com/gabime/spdlog.git
#     GIT_TAG v1.15.3
# )
# FetchContent_MakeAvailable(spdlog)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")
    message(STATUS "Downloading CPM.cmake...")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake"
    )
endif()

include(cmake/CPM.cmake)
CPMAddPackage("gh:gabime/spdlog@1.17.0")

create_spdlog_app(demo_1_simple_log)
create_spdlog_app(demo_2_file_log)
create_spdlog_app(demo_3_async_log)
create_spdlog_app(demo_4_rotating_file_log)
create_spdlog_app(demo_5_multi_sink_log)
