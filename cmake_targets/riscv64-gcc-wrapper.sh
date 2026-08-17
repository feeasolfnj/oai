#!/bin/bash
# Wrapper for riscv64-linux-gnu-gcc that ensures RISC-V include paths
# come BEFORE any host x86_64-linux-gnu paths.
#
# This is necessary because CMake adds -I/usr/include/x86_64-linux-gnu
# to the include path, which causes the compiler to find x86 headers
# instead of RISC-V headers.

# Filter out x86_64 multiarch paths and prepend RISC-V paths
filtered_args=()
prepend_includes="-isystem /home/kongbai/openairinterface5g/cmake_targets/riscv64-stubs/include -isystem /usr/riscv64-linux-gnu/include -isystem /usr/lib/gcc-cross/riscv64-linux-gnu/11/include"

skip_next=false
for arg in "$@"; do
    if $skip_next; then
        skip_next=false
        continue
    fi
    # Skip -I/usr/include/x86_64-linux-gnu
    if [[ "$arg" == "-I/usr/include/x86_64-linux-gnu" ]] || [[ "$arg" == "-isystem/usr/include/x86_64-linux-gnu" ]]; then
        continue
    fi
    # Skip -isystem that follows a removed -I (to avoid orphaned -isystem)
    if [[ "$arg" == "-isystem" ]]; then
        # Check if next arg is the x86 path
        continue
    fi
    filtered_args+=("$arg")
done

exec /usr/bin/riscv64-linux-gnu-gcc $prepend_includes "${filtered_args[@]}"
