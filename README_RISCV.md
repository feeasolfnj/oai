# OAI 5G RISC-V 移植版

> 在 QEMU 仿真环境下，将 OpenAirInterface 5G 移植到 RISC-V 架构，通过 rfsim 实现 UE 接入 gNB 并完成端到端数据传输。

## 这是什么项目？

### 一句话说明

**用软件模拟无线电波，让 5G 基站和手机在 RISC-V 仿真器上完成真实的 5G 通信流程。**

### 详细解释

正常的 5G 通信需要：
- **gNB（基站）**：发射/接收无线电波
- **UE（手机）**：通过无线电波和基站通信
- **核心网（AMF/UPF）**：管理用户、转发数据到互联网

本项目用 **rfsim（软件射频前端）** 代替真实无线电波：
- gNB 和 UE 通过 **TCP socket** 传输 IQ 采样数据
- 用 **QEMU** 在 x86 电脑上运行 RISC-V 程序
- 用 **SIMDE** 库在 RISC-V 上仿真 x86 SIMD 指令
- 使用 **noS1 模式**，不需要核心网

```
┌─────────────── 你的 x86 电脑（Linux）──────────────┐
│                                                     │
│  ┌──────────────┐          ┌──────────────┐        │
│  │ QEMU 仿真     │          │ QEMU 仿真     │        │
│  │ RISC-V 环境   │  rfsim   │ RISC-V 环境   │        │
│  │              │←TCP:4043→│              │        │
│  │  gNB(基站)   │  传IQ采样 │  UE(手机)    │        │
│  │ oaitun_enb1  │          │ oaitun_ue1   │        │
│  │ 10.0.1.1     │          │ 10.0.1.2     │        │
│  └──────────────┘          └──────────────┘        │
│       ↑                          ↑                  │
│    你 ping ─────────────────── 收 ping              │
└─────────────────────────────────────────────────────┘
```

## 已实现的能力

| 能力 | 状态 | 说明 |
|------|------|------|
| RISC-V 交叉编译 | ? | gNB(121MB) + UE(57MB) RISC-V ELF |
| PBCH 同步 | ? | UE 解码 MIB，完成小区同步 |
| SIB1 解码 | ? | UE 解码系统信息，获取 PRACH 配置 |
| 随机接入 (RA) | ? | PRACH Msg1→RAR Msg2→Msg3→Msg4 |
| RRC 连接 | ? | RRCSetup→SecurityMode→RRC CONNECTED |
| DRB 数据通道 | ? | noS1 模式手工建立默认 DRB |
| 下行数据传输 | ? | gNB→5G 协议栈→UE (48 字节) |
| 上行数据传输 | ? | UE→5G 协议栈→gNB (48 字节) |
| Ping 验证 | ? | 0% 丢包，双向通过 5G 栈 |

## 5G 接入完整流程

```
第1步: gNB 启动 → 广播 SSB（同步信号），监听端口 4043
第2步: UE 启动 → 通过 TCP 连接 gNB（rfsim 代替无线电波）
第3步: PBCH 同步 → UE 解码 MIB，知道小区参数
第4步: SIB1 解码 → UE 获取 PRACH 配置
第5步: PRACH Msg1 → UE 发送 "我想接入"
第6步: RAR Msg2 → gNB 回复 "收到，给你临时ID"
第7步: Msg3 → UE 发送 RRCSetupRequest
第8步: Msg4 → gNB 回复 RRCSetup，UE 进入 RRC_CONNECTED
第9步: SecurityModeCommand → 协商加密方式
第10步: DRB 建立 → 建立数据通道
第11步: Ping 测试 → 数据双向通过 5G 协议栈传输
```

## 快速开始

### 系统要求

- **OS**: Ubuntu 22.04 (或 20.04)
- **内存**: 8GB+
- **磁盘**: 20GB 可用空间
- **网络**: 需要能访问 apt 源安装依赖

### 第一步：安装系统依赖

```bash
sudo apt update
sudo apt install qemu-user qemu-user-static
sudo apt install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
sudo apt install libc6-riscv64-cross
sudo apt install cmake build-essential
```

### 第二步：克隆仓库

```bash
git clone -b riscv-port git@github.com:feeasolfnj/oai.git
cd oai
```

### 第三步：一键搭建环境

```bash
./setup.sh
```

这个脚本会自动：
1. 检查系统依赖
2. 准备 RISC-V 库环境（riscv-env/）
3. 检查 stubs_link.o
4. CMake 构建
5. 编译 gNB + UE + rfsimulator

> **注意**：如果 `setup.sh` 提示需要手动添加 `stubs_link.o` 到 link.txt，按提示操作或运行脚本中的 sed 命令。

### 第四步：一键运行 rfsim

```bash
sudo ./run_rfsim.sh full
```

这个脚本会自动：
1. 清理残留进程和 TUN 接口
2. 启动 gNB（基站）
3. 等待 gNB 监听端口 4043
4. 启动 UE（手机）
5. 等待 UE 完成接入（约 60 秒）
6. 检查 RRC CONNECTED 和 DRB 建立
7. 设置 TUN 接口路由

### 第五步：Ping 测试

```bash
sudo ./run_rfsim.sh ping
```

预期输出：
```
PING 10.0.1.2 (10.0.1.2) from 10.0.1.1 : 56(84) bytes of data.
64 bytes from 10.0.1.2: icmp_seq=1 ttl=64 time=0.161 ms
64 bytes from 10.0.1.2: icmp_seq=2 ttl=64 time=0.028 ms
64 bytes from 10.0.1.2: icmp_seq=3 ttl=64 time=0.070 ms

3 packets transmitted, 3 received, 0% packet loss
```

### 第六步：停止

```bash
sudo ./run_rfsim.sh stop
```

## 手动运行方式

如果脚本失败，可以手动操作（参考 [HANDOVER.md](HANDOVER.md)）：

### 启动 gNB
```bash
cd build-riscv
sudo env LD_LIBRARY_PATH=../riscv-env/lib qemu-riscv64 \
  -L /usr/riscv64-linux-gnu ./nr-softmodem \
  -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf \
  --rfsim --sa --noS1 > /tmp/gnb_rfsim.log 2>&1 &
```

### 启动 UE
```bash
cd build-riscv
sudo env LD_LIBRARY_PATH=../riscv-env/lib qemu-riscv64 \
  -L /usr/riscv64-linux-gnu ./nr-uesoftmodem \
  -O ../ci-scripts/conf_files/nrue.band78.106prb.rfsim.conf \
  --rfsim --noS1 --sa -C 3319680000 > /tmp/ue_rfsim.log 2>&1 &
```

### 等待接入（约 60-90 秒）
```bash
grep "NR_RRC_CONNECTED" /tmp/ue_rfsim.log
grep "noS1.*DRB" /tmp/gnb_rfsim.log
```

### 设置路由
```bash
sudo ip route add 10.0.1.2 dev oaitun_enb1 table 10000
sudo ip route add 10.0.1.1 dev oaitun_ue1  table 10000
sudo ip rule add to 10.0.1.2 lookup 10000
sudo ip rule add to 10.0.1.1 lookup 10000
sudo ip rule add from 10.0.1.2 lookup 10000
sudo ip rule add from 10.0.1.1 lookup 10000
```

### Ping 测试
```bash
sudo ping -I 10.0.1.1 -c 3 10.0.1.2
```

## 项目结构

```
oai/
├── README_RISCV.md          ← 本文件
├── HANDOVER.md               ← 详细交接文档（含调试经验）
├── setup.sh                  ← 环境搭建脚本
├── run_rfsim.sh              ← rfsim 运行脚本
├── riscv-env/                ← RISC-V 运行环境
│   ├── lib/                  ← RISC-V 共享库 (8.3MB)
│   ├── include/              ← 头文件
│   ├── stubs_link.c          ← 链接桩源码
│   ├── stubs_link.o          ← 预编译链接桩
│   ├── stubs.c               ← 其他桩
│   └── zlib_stub.c           ← zlib 桩
├── cmake_targets/
│   ├── riscv64-toolchain.cmake   ← RISC-V 交叉编译配置
│   └── riscv64-stubs/            ← BLAS/OpenSSL 桩
├── ci-scripts/conf_files/
│   ├── gnb.sa.band78.106prb.rfsim.conf  ← gNB 配置
│   └── nrue.band78.106prb.rfsim.conf    ← UE 配置
├── openair1/PHY/NR_UE_TRANSPORT/
│   ├── nr_dlsch_demodulation.c        ← PDSCH 解调（含 SIMD 修复）
│   └── nr_dlsch_llr_computation.c     ← LLR 计算（含 SIMD 修复）
├── openair2/RRC/NR/
│   └── rrc_gNB.c                      ← gNB RRC（含 noS1 DRB 补全）
├── openair2/SDAP/nr_sdap/
│   └── nr_sdap_entity.c               ← SDAP（含 noS1 TUN 写入修复）
└── openair2/LAYER2/nr_pdcp/
    └── nr_pdcp_oai_api.c              ← PDCP（含 TUN 读取日志）
```

## 关键技术点

### 1. SIMD 指令仿真（修复 PDSCH LLR 全零）

**问题**：OAI 5G 的 PHY 层大量使用 x86 SSE2 指令，RISC-V 不支持。

**解决**：
- 使用 SIMDE 库在 RISC-V 上仿真 x86 SSE2
- 在 29 处 `#if` 预处理器条件中添加 `|| defined(OAI_SIMD_X86_EMULATION)`
- 编译时定义 `OAI_SIMD_X86_EMULATION` 宏

**关键文件**：
- [nr_dlsch_demodulation.c](openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_demodulation.c)
- [nr_dlsch_llr_computation.c](openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_llr_computation.c)

### 2. noS1 模式协议栈补全

**问题**：noS1 模式无核心网，正常由 AMF 触发的 SecurityModeCommand 和 DRB 建立流程不存在。

**解决**：在 [rrc_gNB.c](openair2/RRC/NR/rrc_gNB.c) 中：
- 检查 `IS_SOFTMODEM_NOS1`，手工触发 SecurityModeCommand
- 手工创建默认 DRB（PDU session id=10, QFI=9）
- 将 RLC bearer 加入 `masterCellGroup->rlc_BearerToAddModList`

### 3. SDAP 上行数据路径修复

**问题**：noS1 模式下，gNB 收到 UE 上行数据后发往不存在的 GTP-U，数据丢失。

**解决**：在 [nr_sdap_entity.c](openair2/SDAP/nr_sdap/nr_sdap_entity.c) 中：
- noS1 模式下直接 `write(nas_sock_fd[0], ...)` 写入 gNB TUN 接口
- 让 Linux 网络栈处理数据转发

### 4. 39 个协议文件桩化

为了简化编译，将 39 个核心网/CU-DU 分离协议文件替换为空实现：
- NGAP（gNB?5GC）、S1AP（eNB?EPC）
- F1AP（CU?DU）、E1AP（CU-CP?CU-UP）
- M2AP/M3AP（MBMS 多播）

原始代码存为 `.c.orig`，未来可恢复。

## noS1 模式说明

### 什么是 noS1 模式？

| 正常 5G | noS1 模式 |
|---------|-----------|
| gNB ? AMF(核心网) ? UPF ? 互联网 | gNB 直接通过 TUN 网卡收发数据 |
| 需要 AMF/UPF/SMF | 不需要核心网 |
| UE 可以上互联网 | UE 只能和 gNB 互相 ping |
| 有鉴权/加密管理 | 简化版，手工补全 |

### noS1 能做什么？

- ? 验证 5G PHY（物理层 OFDM/LDPC/SIMD）
- ? 验证 L2/L3（MAC/RLC/PDCP/RRC）
- ? 测试 UE 接入 gNB 的完整流程
- ? 端到端 IP 数据双向传输
- ? RISC-V 架构可行性验证

### noS1 不能做什么？

- ? 上互联网（没有 UPF 做 NAT/路由）
- ? UE 注册鉴权（没有 AMF 做 USIM 鉴权）
- ? 多 UE 切换（没有核心网管理移动性）

## 验证日志

成功运行后，日志会显示：

**接入流程**：
```
UE: pbch decoded sucessfully
UE: State = NR_RRC_CONNECTED
gNB: noS1: created default DRB 1 for UE 75b2
gNB: noS1: added RLC bearer for DRB 1 (lcid 4) to masterCellGroup
gNB: Received NR_RRCReconfigurationComplete from UE rnti 75b2
```

**数据传输**：
```
gNB: enb_tun_read: has_ue=1, ue_id=75b2
gNB: sdap_data_req returned 1
UE:  deliver_sdu_drb: IP packet received size 48
UE:  UE TUN write: len=48
gNB: gNB TUN write (noS1): len=48
```

## 常见问题

### Q: QEMU 运行速度慢吗？
A: QEMU 用户态仿真约 17% 实时速度，但 rfsim 的时序比真实空口宽松，RA 流程能顺利完成。

### Q: 为什么 gNB 必须用 root 运行？
A: gNB 需要创建 TUN 虚拟网卡，这需要 `CAP_NET_ADMIN` 权限。建议同时启用 `SCHED_FIFO` 实时调度。

### Q: 为什么 Ping 延迟只有 0.0xx ms？
A: 两个 TUN 网卡在同一台主机上，Linux 可能走本地路由捷径。设置 `ip rule + table 10000` 可以强制数据走 5G 栈。日志中的 `gNB TUN write` 和 `UE TUN write` 证明数据确实经过了 5G 协议栈。

### Q: 编译失败怎么办？
A: 参见 [HANDOVER.md](HANDOVER.md) 第四节"已知构建坑"，常见问题：
1. `stubs_link.o` 未加入 link.txt → 手动添加
2. `libasn1_nr_rrc.a` 偶发错误 → 手动跑 link.txt
3. `librfsimulator.so` 不在默认目标 → 单独 `make rfsimulator`

### Q: UE 连不上 gNB？
A: 检查：
1. gNB 是否已启动并监听端口 4043
2. UE 频率是否匹配：`-C 3319680000`（3319.68 MHz）
3. 防火墙是否阻止 localhost TCP 4043

## 后续路线图

如果要实现 UE 上互联网，需要：
1. 部署核心网（free5GC 或 Open5GS）
2. 补全 NGAP 协议（8 个文件）
3. 补全 NAS 协议（3-4 个文件）
4. 去掉 `--noS1` 参数，切换到正常 S1 模式

详见 [HANDOVER.md](HANDOVER.md) 第十一章"协议栈补全路线图"。

## 许可证

继承 OAI Public License V1.1，见 [LICENSE](LICENSE)。

## 致谢

- [OpenAirInterface 5G](https://openairinterface.org/) 原始项目
- [SIMDE](https://github.com/simd-everywhere/simde) SIMD 仿真库
- [QEMU](https://www.qemu.org/) RISC-V 仿真器
