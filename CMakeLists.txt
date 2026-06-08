cmake_minimum_required(VERSION 3.21)
project(wesl-distribution LANGUAGES C CXX)

set(WESL_VERSION "v0.4.0" CACHE STRING "wesl-c version to fetch")
set(WESL_BINARY_MIRROR "https://github.com/vxjnc/wesl-distribution")
set(WESL_LINK_TYPE "SHARED" CACHE STRING "Library type to link: SHARED or STATIC")

# --- Detect OS ---
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WESL_OS "windows")
    set(WESL_DYNLIB "wesl_c.dll")
    set(WESL_STATICLIB "wesl_c.lib")
    set(WESL_IMPLIB "wesl_c.dll.lib") # not produced by Rust, use staticlib on Windows if needed
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(WESL_OS "linux")
    set(WESL_DYNLIB "libwesl_c.so")
    set(WESL_STATICLIB "libwesl_c.a")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(WESL_OS "macos")
    set(WESL_DYNLIB "libwesl_c.dylib")
    set(WESL_STATICLIB "libwesl_c.a")
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
if(WESL_LINK_TYPE STREQUAL "STATIC")
    add_library(wesl STATIC IMPORTED GLOBAL)
    set_target_properties(wesl PROPERTIES
        IMPORTED_LOCATION "${wesl_bin_SOURCE_DIR}/${WESL_STATICLIB}"
    )
else()
    add_library(wesl SHARED IMPORTED GLOBAL)
    set_target_properties(wesl PROPERTIES
        IMPORTED_LOCATION "${wesl_bin_SOURCE_DIR}/${WESL_DYNLIB}"
    )
    # On Windows, SHARED imported targets need IMPORTED_IMPLIB.
    # Rust cdylib does not produce a .dll.lib by default.
    # Use STATIC if you need Windows import lib, or generate one manually.
endif()

set_target_properties(wesl PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${wesl_bin_SOURCE_DIR}"
)
