cmake_minimum_required(VERSION 3.21)
project(wesl-test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(wesl
    GIT_REPOSITORY https://github.com/vxjnc/wesl-distribution
    GIT_TAG main
)
FetchContent_MakeAvailable(wesl)

add_executable(test main.c)
target_link_libraries(test PRIVATE wesl)

target_copy_wesl_binaries(test)
