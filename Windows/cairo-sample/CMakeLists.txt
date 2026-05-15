cmake_minimum_required(VERSION 3.15)
project(CairoExample C)

# 1. Find the PkgConfig module
find_package(PkgConfig REQUIRED)

# 2. Point to the vcpkg .pc file location (this helps if it's not found automatically)
# The toolchain usually handles this, but we can force it:
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-windows/lib/pkgconfig")

# 3. Search for cairo using pkg-config
# The 'IMPORTED_TARGET' flag creates a handy 'PkgConfig::CAIRO' target
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairo)

add_executable(cairo_sample main.c)

# 4. Link to the PkgConfig target
target_link_libraries(cairo_sample PRIVATE PkgConfig::CAIRO)
