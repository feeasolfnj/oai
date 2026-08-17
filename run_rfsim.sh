#!/bin/bash
# OAI 5G RISC-V 移植版 - rfsim 一键运行脚本
# 用法: sudo ./run_rfsim.sh [gnb_only|ue_only|full|ping|stop]
set -e

REPO_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$REPO_DIR/build-riscv"
RISCV_ENV="$REPO_DIR/riscv-env"
GNB_LOG="/tmp/gnb_rfsim.log"
UE_LOG="/tmp/ue_rfsim.log"

# 检查是否 root
if [ "$EUID" -ne 0 ]; then
    echo "请用 sudo 运行: sudo $0 $@"
    exit 1
fi

# 检查二进制
if [ ! -f "$BUILD_DIR/nr-softmodem" ] || [ ! -f "$BUILD_DIR/nr-uesoftmodem" ]; then
    echo "错误：二进制不存在，请先运行 ./setup.sh"
    exit 1
fi

export LD_LIBRARY_PATH="$RISCV_ENV/lib"
QEMU_CMD="qemu-riscv64 -L /usr/riscv64-linux-gnu"

case "${1:-full}" in
    stop)
        echo "停止所有进程..."
        pkill -f nr-uesoftmodem 2>/dev/null || true
        pkill -f "qemu.*nr-softmodem" 2>/dev/null || true
        pkill -f "qemu.*nr-uesoftmodem" 2>/dev/null || true
        sleep 2
        ip link delete oaitun_enb1 2>/dev/null || true
        ip link delete oaitun_ue1 2>/dev/null || true
        echo "已停止并清理"
        ;;

    gnb_only)
        echo "启动 gNB（基站）..."
        pkill -f "qemu.*nr-softmodem" 2>/dev/null || true
        sleep 1
        cd "$BUILD_DIR"
        $QEMU_CMD ./nr-softmodem \
            -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf \
            --rfsim --sa --noS1 > "$GNB_LOG" 2>&1 &
        echo "gNB 已启动 (PID: $!), 日志: $GNB_LOG"
        echo "等待端口 4043 监听..."
        sleep 10
        grep -i "waiting\|rfsim\|active_clients" "$GNB_LOG" | tail -3
        ;;

    ue_only)
        echo "启动 UE（手机）..."
        pkill -f "qemu.*nr-uesoftmodem" 2>/dev/null || true
        sleep 1
        cd "$BUILD_DIR"
        $QEMU_CMD ./nr-uesoftmodem \
            -O ../ci-scripts/conf_files/nrue.band78.106prb.rfsim.conf \
            --rfsim --noS1 --sa -C 3319680000 > "$UE_LOG" 2>&1 &
        echo "UE 已启动 (PID: $!), 日志: $UE_LOG"
        ;;

    full)
        echo "============================================"
        echo "  OAI 5G RISC-V rfsim 完整运行"
        echo "============================================"
        
        # 清理
        echo "[1/5] 清理残留..."
        pkill -f nr-uesoftmodem 2>/dev/null || true
        pkill -f "qemu.*nr-softmodem" 2>/dev/null || true
        pkill -f "qemu.*nr-uesoftmodem" 2>/dev/null || true
        sleep 2
        ip link delete oaitun_enb1 2>/dev/null || true
        ip link delete oaitun_ue1 2>/dev/null || true
        rm -f "$GNB_LOG" "$UE_LOG"
        
        # 启动 gNB
        echo "[2/5] 启动 gNB（基站）..."
        cd "$BUILD_DIR"
        $QEMU_CMD ./nr-softmodem \
            -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf \
            --rfsim --sa --noS1 > "$GNB_LOG" 2>&1 &
        GNB_PID=$!
        echo "  gNB PID: $GNB_PID"
        echo "  等待 gNB 启动 (15秒)..."
        sleep 15
        if ! grep -q "rfsim" "$GNB_LOG"; then
            echo "  ? gNB 启动失败，查看日志: $GNB_LOG"
            exit 1
        fi
        echo "  ? gNB 已启动，监听端口 4043"
        
        # 启动 UE
        echo "[3/5] 启动 UE（手机）..."
        $QEMU_CMD ./nr-uesoftmodem \
            -O ../ci-scripts/conf_files/nrue.band78.106prb.rfsim.conf \
            --rfsim --noS1 --sa -C 3319680000 > "$UE_LOG" 2>&1 &
        UE_PID=$!
        echo "  UE PID: $UE_PID"
        
        # 等待接入
        echo "[4/5] 等待 UE 接入 (60秒)..."
        for i in $(seq 1 60); do
            if grep -q "NR_RRC_CONNECTED" "$UE_LOG" 2>/dev/null; then
                echo "  ? UE 已进入 RRC CONNECTED 状态 (${i}秒)"
                break
            fi
            sleep 1
        done
        
        # 检查 DRB
        if grep -q "noS1.*created default DRB" "$GNB_LOG" 2>/dev/null; then
            echo "  ? DRB 已建立"
        else
            echo "  ? DRB 尚未建立，等待更多时间..."
            sleep 20
        fi
        
        # 设置路由
        echo "[5/5] 设置路由..."
        ip addr show oaitun_enb1 2>/dev/null | grep inet | head -1
        ip addr show oaitun_ue1 2>/dev/null | grep inet | head -1
        ip route add 10.0.1.2 dev oaitun_enb1 table 10000 2>/dev/null || true
        ip route add 10.0.1.1 dev oaitun_ue1  table 10000 2>/dev/null || true
        ip rule add to 10.0.1.2 lookup 10000 2>/dev/null || true
        ip rule add to 10.0.1.1 lookup 10000 2>/dev/null || true
        ip rule add from 10.0.1.2 lookup 10000 2>/dev/null || true
        ip rule add from 10.0.1.1 lookup 10000 2>/dev/null || true
        echo "  ? 路由设置完成"
        
        echo ""
        echo "============================================"
        echo "  rfsim 已就绪！"
        echo "============================================"
        echo ""
        echo "gNB 日志: $GNB_LOG"
        echo "UE  日志: $UE_LOG"
        echo ""
        echo "Ping 测试:"
        echo "  ping -I 10.0.1.1 -c 3 10.0.1.2"
        echo ""
        echo "停止: sudo $0 stop"
        ;;

    ping)
        echo "Ping 测试 (gNB 10.0.1.1 -> UE 10.0.1.2)..."
        ping -I 10.0.1.1 -c 3 10.0.1.2
        echo ""
        echo "=== 数据走 5G 栈验证 ==="
        echo "--- 下行 (gNB→UE) ---"
        grep -E "enb_tun_read: has_ue=1|sdap_data_req returned 1|UE TUN write" "$GNB_LOG" "$UE_LOG" 2>/dev/null | tail -5
        echo ""
        echo "--- 上行 (UE→gNB) ---"
        grep -E "gNB TUN write|deliver_sdu_drb.*IP packet" "$GNB_LOG" "$UE_LOG" 2>/dev/null | tail -5
        ;;

    status)
        echo "=== gNB 状态 ==="
        grep -iE "active_clients|noS1.*DRB|SecurityMode|ReconfigurationComplete" "$GNB_LOG" 2>/dev/null | tail -5
        echo ""
        echo "=== UE 状态 ==="
        grep -iE "pbch decoded|NR_RRC_CONNECTED|ReconfigurationComplete|SecurityMode" "$UE_LOG" 2>/dev/null | tail -5
        echo ""
        echo "=== TUN 接口 ==="
        ip addr show oaitun_enb1 2>/dev/null | grep inet | head -1
        ip addr show oaitun_ue1 2>/dev/null | grep inet | head -1
        ;;

    *)
        echo "用法: sudo $0 [full|gnb_only|ue_only|ping|status|stop]"
        echo ""
        echo "  full      - 完整运行（清理+gNB+UE+路由）"
        echo "  gnb_only  - 只启动 gNB"
        echo "  ue_only   - 只启动 UE"
        echo "  ping      - Ping 测试 + 5G 栈验证"
        echo "  status    - 查看运行状态"
        echo "  stop      - 停止所有进程并清理"
        ;;
esac
