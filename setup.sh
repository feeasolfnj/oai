#!/bin/bash
# OAI 5G RISC-V 移植版 - 环境搭建脚本
# 用法: ./setup.sh
set -e

REPO_DIR="$(cd "$(dirname "$0")" && pwd)"
RISCV_ENV="$REPO_DIR/riscv-env"
BUILD_DIR="$REPO_DIR/build-riscv"

echo "============================================"
echo "  OAI 5G RISC-V 移植版 - 环境搭建"
echo "============================================"
echo ""

# 1. 检查系统依赖
echo "[1/5] 检查系统依赖..."
check_cmd() {
    if command -v "$1" &>/dev/null; then
        echo "  ? $1 已安装"
        return 0
    else
        echo "  ? $1 未安装"
        return 1
    fi
}

MISSING=0
check_cmd qemu-riscv64 || MISSING=1
check_cmd riscv64-linux-gnu-gcc || MISSING=1

if [ $MISSING -eq 1 ]; then
    echo ""
    echo "缺少系统依赖，请先安装："
    echo "  sudo apt update"
    echo "  sudo apt install qemu-user qemu-user-static gcc-riscv64-linux-gnu"
    echo "  sudo apt install libc6-riscv64-cross gcc-riscv64-linux-gnu g++-riscv64-linux-gnu"
    echo ""
    echo "还需要 RISC-V sysroot（/usr/riscv64-linux-gnu）："
    echo "  sudo apt install libc6-riscv64-cross"
    echo ""
    echo "安装完成后重新运行此脚本。"
    exit 1
fi

echo ""

# 2. 准备 riscv-libs
echo "[2/5] 准备 RISC-V 库环境..."
if [ ! -d "/home/kongbai/riscv-libs" ]; then
    # 如果不是 kongbai 用户，创建符号链接或设置路径
    echo "  创建 riscv-libs 链接..."
    # 使用仓库内的 riscv-env 作为 riscv-libs
    export RISCV_LIBS="$RISCV_ENV"
else
    export RISCV_LIBS="/home/kongbai/riscv-libs"
fi
echo "  RISCV_LIBS = $RISCV_LIBS"
echo ""

# 3. 编译 stubs_link.o（如果需要重新编译）
echo "[3/5] 检查 stubs_link.o..."
if [ ! -f "$RISCV_ENV/stubs_link.o" ]; then
    echo "  编译 stubs_link.o..."
    riscv64-linux-gnu-gcc -c -march=rv64gcv -mabi=lp64d \
        -isystem "$REPO_DIR/cmake_targets/riscv64-stubs/include" \
        -isystem /usr/riscv64-linux-gnu/include \
        "$RISCV_ENV/stubs_link.c" -o "$RISCV_ENV/stubs_link.o"
    echo "  ? stubs_link.o 编译完成"
else
    echo "  ? stubs_link.o 已存在"
fi
echo ""

# 4. CMake 构建
echo "[4/5] CMake 构建..."
if [ ! -d "$BUILD_DIR" ]; then
    echo "  创建 build-riscv 目录..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../cmake_targets/riscv64-toolchain.cmake \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo
    echo ""
    echo "  ★ 重要：需要手动将 stubs_link.o 加入链接命令"
    echo "    编辑以下文件，在链接命令中加入 stubs_link.o 路径："
    echo "    $BUILD_DIR/CMakeFiles/nr-softmodem.dir/link.txt"
    echo "    $BUILD_DIR/CMakeFiles/nr-uesoftmodem.dir/link.txt"
    echo "    加入: $RISCV_ENV/stubs_link.o"
    echo ""
    echo "  也可以运行以下命令自动添加："
    echo "    sed -i 's|\\$<TARGET_FILE:|\\$RISCV_ENV/stubs_link.o \$<TARGET_FILE:|g' \\"
    echo "      $BUILD_DIR/CMakeFiles/nr-softmodem.dir/link.txt \\"
    echo "      $BUILD_DIR/CMakeFiles/nr-uesoftmodem.dir/link.txt"
else
    echo "  build-riscv 目录已存在，跳过 cmake"
fi
echo ""

# 5. 编译目标
echo "[5/5] 编译二进制..."
cd "$BUILD_DIR"

echo "  编译 nr-softmodem (gNB)..."
make nr-softmodem -j$(nproc) 2>&1 | tail -3

echo "  编译 nr-uesoftmodem (UE)..."
make nr-uesoftmodem -j$(nproc) 2>&1 | tail -3

echo "  编译 rfsimulator..."
make rfsimulator -j$(nproc) 2>&1 | tail -3

echo ""
echo "============================================"
echo "  构建完成！"
echo "============================================"
echo ""
echo "二进制位置："
ls -la "$BUILD_DIR/nr-softmodem" "$BUILD_DIR/nr-uesoftmodem" 2>&1
echo ""
echo "运行方式："
echo "  sudo ./run_rfsim.sh"
echo ""
echo "或手动运行（见 HANDOVER.md 第五节）："
echo "  gNB: LD_LIBRARY_PATH=$RISCV_ENV/lib qemu-riscv64 -L /usr/riscv64-linux-gnu ./build-riscv/nr-softmodem -O ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf --rfsim --sa --noS1"
echo "  UE:  LD_LIBRARY_PATH=$RISCV_ENV/lib qemu-riscv64 -L /usr/riscv64-linux-gnu ./build-riscv/nr-uesoftmodem -O ci-scripts/conf_files/nrue.band78.106prb.rfsim.conf --rfsim --noS1 --sa -C 3319680000"
