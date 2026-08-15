mkdir build
cd build
cmake .. `
  -DCMAKE_TOOLCHAIN_FILE="C:/Users/netka/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -A x64 `
  -DPKG_CONFIG_EXECUTABLE="C:/Users/netka/vcpkg/installed/x64-windows/tools/pkgconf/pkgconf.exe"

# Build
cmake --build . --config Release