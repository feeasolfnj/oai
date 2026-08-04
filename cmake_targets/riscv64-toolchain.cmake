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
# Common toolchain installations:
#   - Ubuntu/Debian: riscv64-linux-gnu-gcc / riscv64-linux-gnu-g++
#   - RISC-V official: /opt/riscv/bin/riscv64-unknown-linux-gnu-gcc
set(CMAKE_C_COMPILER   riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER riscv64-linux-gnu-g++)
set(CMAKE_AR           riscv64-linux-gnu-ar  CACHE FILEPATH "Archiver")
set(CMAKE_RANLIB       riscv64-linux-gnu-ranlib CACHE FILEPATH "Ranlib")
set(CMAKE_STRIP        riscv64-linux-gnu-strip CACHE FILEPATH "Strip")

# RISC-V specific compile flags
set(CMAKE_C_FLAGS_INIT   "-march=rv64gcv -mabi=lp64d")
set(CMAKE_CXX_FLAGS_INIT "-march=rv64gcv -mabi=lp64d")

# Sysroot (uncomment and adjust if needed)
# set(CMAKE_SYSROOT /opt/riscv/sysroot)

# Search path configuration
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
