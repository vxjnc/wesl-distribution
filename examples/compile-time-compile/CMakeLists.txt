cmake_minimum_required(VERSION 3.21)
project(wesl-test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

include(FetchContent)
FetchContent_Declare(wesl
    GIT_REPOSITORY https://github.com/vxjnc/wesl-distribution
    GIT_TAG main
)
FetchContent_MakeAvailable(wesl)

include("cmake/shaders.cmake")

add_executable(test main.c)
target_link_libraries(test PRIVATE wesl)

add_dependencies(test wgsl_shaders)

target_include_directories(test PRIVATE "${GENERATED_DIR}/")

target_copy_wesl_binaries(test)
