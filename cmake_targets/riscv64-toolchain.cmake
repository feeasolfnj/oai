# RISC-V 64-bit cross-compilation toolchain for OpenAirInterface
#
# Usage:
#   cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake_targets/riscv64-toolchain.cmake
#
# Or via build_oai:
#   ./build_oai --gNB --nrUE --cmake-opt "-DCMAKE_TOOLCHAIN_FILE=../riscv64-toolchain.cmake"

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# Cross-compiler paths (adjust to match your RISC-V toolchain installation)
set(CMAKE_C_COMPILER   riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER riscv64-linux-gnu-g++)
set(CMAKE_AR           riscv64-linux-gnu-ar  CACHE FILEPATH "Archiver")
set(CMAKE_RANLIB       riscv64-linux-gnu-ranlib CACHE FILEPATH "Ranlib")
set(CMAKE_STRIP        riscv64-linux-gnu-strip CACHE FILEPATH "Strip")

# Path to OpenSSL stubs for RISC-V cross-compilation
set(STUBS_DIR "${CMAKE_CURRENT_LIST_DIR}/riscv64-stubs")

# RISC-V specific compile flags with OpenSSL stubs include path
# The riscv64-stubs/include directory contains local overrides for missing
# system headers (gnu/stubs.h, etc.) that are needed because CMake's
# pkg_check_modules adds -I/usr/include/x86_64-linux-gnu for x86 host libraries.
# By putting the stubs directory FIRST in the include path, our overrides
# are found before the x86 system headers.
set(CMAKE_C_FLAGS_INIT   "-isystem ${STUBS_DIR}/include -isystem /usr/riscv64-linux-gnu/include -isystem /usr/lib/gcc-cross/riscv64-linux-gnu/11/include -march=rv64gcv -mabi=lp64d")
set(CMAKE_CXX_FLAGS_INIT "-isystem ${STUBS_DIR}/include -isystem /usr/riscv64-linux-gnu/include -isystem /usr/lib/gcc-cross/riscv64-linux-gnu/11/include -march=rv64gcv -mabi=lp64d")

# Ensure stubs include path comes FIRST to override any x86_64-linux-gnu paths
set(CMAKE_C_FLAGS   "-isystem ${STUBS_DIR}/include -isystem /usr/riscv64-linux-gnu/include -isystem /usr/lib/gcc-cross/riscv64-linux-gnu/11/include")
set(CMAKE_CXX_FLAGS "-isystem ${STUBS_DIR}/include -isystem /usr/riscv64-linux-gnu/include -isystem /usr/lib/gcc-cross/riscv64-linux-gnu/11/include")

# Do NOT set CMAKE_SYSROOT - it breaks library resolution
# The cross-compiler already knows where to find RISC-V libraries

# Search path configuration for CMake's find operations
set(CMAKE_FIND_ROOT_PATH /usr/riscv64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Add stubs directory to pkg-config search path
set(ENV{PKG_CONFIG_PATH} "${STUBS_DIR}:$ENV{PKG_CONFIG_PATH}")
