cmake_minimum_required(VERSION 3.21)
project(wesl-distribution LANGUAGES C CXX)

set(WESL_VERSION "v0.4.0" CACHE STRING "wesl-c version to fetch")
set(WESL_BINARY_MIRROR "https://github.com/vxjnc/wesl-distribution")
set(WESL_LINK_TYPE "STATIC" CACHE STRING "Library type to link: SHARED or STATIC")

# --- Detect OS ---
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WESL_OS "windows")
    set(WESL_DYNLIB "wesl_c.dll")
    set(WESL_STATICLIB "wesl_c.lib")
    set(WESL_EXE "wesl.exe")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(WESL_OS "linux")
    set(WESL_DYNLIB "libwesl_c.so")
    set(WESL_STATICLIB "libwesl_c.a")
    set(WESL_EXE "wesl")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(WESL_OS "macos")
    set(WESL_DYNLIB "libwesl_c.dylib")
    set(WESL_STATICLIB "libwesl_c.a")
    set(WESL_EXE "wesl")
else()
    message(FATAL_ERROR "wesl-distribution: unsupported OS '${CMAKE_SYSTEM_NAME}'")
endif()

# --- Detect arch ---
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(WESL_ARCH "arm64")
else()
    set(WESL_ARCH "x86_64")
endif()

set(WESL_ZIP_NAME "wesl-${WESL_OS}-${WESL_ARCH}")
set(WESL_URL "${WESL_BINARY_MIRROR}/releases/download/${WESL_VERSION}/${WESL_ZIP_NAME}.zip")

message(STATUS "wesl-distribution: fetching ${WESL_URL}")

include(FetchContent)
FetchContent_Declare(wesl_bin URL ${WESL_URL})
FetchContent_MakeAvailable(wesl_bin)

# --- Create imported target ---
if(WESL_LINK_TYPE STREQUAL "SHARED")
    add_library(wesl SHARED IMPORTED GLOBAL)
    set_target_properties(wesl PROPERTIES
        IMPORTED_LOCATION "${wesl_bin_SOURCE_DIR}/${WESL_DYNLIB}"
    )
else()
    add_library(wesl STATIC IMPORTED GLOBAL)
    set_target_properties(wesl PROPERTIES
        IMPORTED_LOCATION "${wesl_bin_SOURCE_DIR}/${WESL_STATICLIB}"
    )
    if(UNIX AND NOT APPLE)
        set_target_properties(wesl PROPERTIES
            INTERFACE_LINK_LIBRARIES "m"
        )
    endif()
endif()

set_target_properties(wesl PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${wesl_bin_SOURCE_DIR}"
)

# --- Create imported target for CLI ---
add_executable(wesl-cli IMPORTED GLOBAL)
set_target_properties(wesl-cli PROPERTIES
    IMPORTED_LOCATION "${wesl_bin_SOURCE_DIR}/${WESL_EXE}"
)

# --- Helper: copy .so/.dll next to the target binary ---
# Usage: target_copy_wesl_binaries(your_target)
function(target_copy_wesl_binaries TARGET)
    if(NOT WESL_LINK_TYPE STREQUAL "SHARED")
        return()
    endif()
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${wesl_bin_SOURCE_DIR}/${WESL_DYNLIB}"
            "$<TARGET_FILE_DIR:${TARGET}>"
        COMMENT "Copying ${WESL_DYNLIB} to output directory"
    )
endfunction()
