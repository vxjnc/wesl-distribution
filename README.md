 # wesl-distribution

Prebuilt binaries of [wesl-c](https://github.com/webgpu-tools/wesl-rs/tree/main/crates/wesl-c) for C/C++ projects, with a CMake integration — no Rust or Cargo required.

Inspired by [WebGPU-distribution](https://github.com/eliemichel/WebGPU-distribution).

## Usage

```cmake
include(FetchContent)
FetchContent_Declare(wesl
    GIT_REPOSITORY https://github.com/vxjnc/wesl-distribution
    GIT_TAG        main
)
FetchContent_MakeAvailable(wesl)

target_link_libraries(your_target PRIVATE wesl)
```

By default links the shared library. To use static:

```cmake
set(WESL_LINK_TYPE "STATIC" CACHE STRING "" FORCE)
```

## Platforms

| OS      | Arch    | Dynamic       | Static       |
|---------|---------|---------------|--------------|
| Linux   | x86_64  | libwesl_c.so  | libwesl_c.a  |
| Windows | x86_64  | wesl_c.dll    | wesl_c.lib   |
| macOS   | arm64   | libwesl_c.dylib | libwesl_c.a |
| macOS   | x86_64  | libwesl_c.dylib | libwesl_c.a |

## Building

Binaries are built via GitHub Actions from the official [wesl-rs](https://github.com/webgpu-tools/wesl-rs) source.
To build a new version, trigger the `Build wesl-c` workflow with the desired tag.
