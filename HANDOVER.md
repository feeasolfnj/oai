# OAI RISC-V 移植项目交接文档（详细版）

> 交接日期：2026-08-16
> 项目路径：`/home/kongbai/openairinterface5g`
> sudo 密码：`123123`（gNB 必须 root 运行）

---

## ? 一句话现状

**全部完成。** QEMU 上 RISC-V OAI rfsim 已跑通完整链路：PBCH 同步 → SIB1 → PRACH → RAR → Msg3/Msg4 → SecurityMode → RRC CONNECTED → DRB 建立 → 双向 IP 数据传输（下行 gNB→UE + 上行 UE→gNB，均通过 5G 协议栈验证）。

**最新修复**（2026-08-17）：noS1 模式下 gNB 上行数据路径——将原本发往不存在的 GTP-U 的上行数据改为直接写入 gNB TUN 接口（[nr_sdap_entity.c](file:///home/kongbai/openairinterface5g/openair2/SDAP/nr_sdap/nr_sdap_entity.c)），修复后 ping 双向通过 5G 栈验证成功。

---

## 一、项目目标

将 OpenAirInterface 5G（OAI）移植到 RISC-V 架构，在 QEMU 仿真环境下：
1. 交叉编译 RISC-V 版本的 `nr-softmodem`（gNB）和 `nr-uesoftmodem`（UE）
2. 用 rfsim（软件射频前端，socket 传 IQ 样点）替代物理 SDR
3. 实现 UE 通过 rfsim 接入 gNB，完成随机接入（RA）和 RRC 连接建立

**架构**：x86 主机交叉编译 → RISC-V ELF → QEMU 用户态仿真运行 → rfsim socket 互联

---

## 二、当前进度（详细）

### ? 已完成的工作

| 模块 | 完成内容 | 关键文件 |
|------|---------|---------|
| 交叉编译 | gNB(121MB) + UE(57MB) 编译成功，RISC-V ELF | `build-riscv/nr-softmodem`, `nr-uesoftmodem` |
| ASN.1 修复 | 解决 asn1c 版本差异导致的 union/ANY_t/ext1 编译错误 | [rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_UE.c), [rrc_timers_and_constants.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_timers_and_constants.c) |
| SIMDE 适配 | x86 SIMD 指令通过 SIMDE 在 RISC-V 仿真 | [sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h), [simde/sse2.h](file:///home/kongbai/openairinterface5g/extra/simde/simde/x86/sse2.h) |
| **PDSCH LLR 全零修复** | **29 处 `#if` 添加 `OAI_SIMD_X86_EMULATION`，启用 SIMD 代码** | [nr_dlsch_demodulation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_demodulation.c), [nr_dlsch_llr_computation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_llr_computation.c) |
| 链接修复 | stubs_link.o 解决 `__builtin_cpu_*`/`der_*`/`ber_*`/`OPENSSL_assert` 未定义符号 | `/home/kongbai/riscv-libs/stubs_link.o` |
| 协议 stub | 39 个协议文件（E1AP/F1AP/M2AP/M3AP/S1AP/NGAP）替换为静态零初始化实现，原始存为 `.c.orig` | `find . -name "*.c.orig"` |
| gNB rfsim 启动 | 端口 4043 监听，RU 线程同步，SIB1/PRACH 持续调度 | [gnb.sa.band78.106prb.rfsim.conf](file:///home/kongbai/openairinterface5g/ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf) |
| UE 同步 | PBCH 译码成功、MIB 解码、`init_sync_frame=0` | — |
| **SIB1 解码** | **PDSCH LLR 修复后 SIB1 PDU 内容非零，RRC 提取 PRACH 配置** | — |
| **PRACH 随机接入** | **Msg1→Msg2(RAR)→Msg3→Msg4 完整 RA 流程** | [nr_ra_procedures.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_UE/nr_ra_procedures.c) |
| **RRC 连接建立** | **RRCSetup→SecurityModeCommand→SecurityModeComplete→RRC CONNECTED** | [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c), [rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_UE.c) |
| **DRB 建立** | **noS1 模式手工创建默认 DRB + RLC bearer，RRCReconfiguration→ReconfigurationComplete** | [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c) |
| **下行数据传输** | **gNB TUN→PDCP→RLC→MAC→PHY→rfsim→UE TUN（48 字节验证）** | [nr_pdcp_oai_api.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/nr_pdcp/nr_pdcp_oai_api.c) |
| **上行数据传输** | **UE TUN→5G 栈→gNB TUN（SDAP 写 TUN 替代 GTP-U，48 字节验证）** | [nr_sdap_entity.c](file:///home/kongbai/openairinterface5g/openair2/SDAP/nr_sdap/nr_sdap_entity.c) |
| rfsim 稳定性 | 修复 epoll 错误崩溃、socketError 中 exit(1) 问题 | [simulator.c](file:///home/kongbai/openairinterface5g/radio/rfsimulator/simulator.c) |
| SCTP/noS1 | noS1 模式跳过 SCTP 任务创建 | [nr-softmodem.c](file:///home/kongbai/openairinterface5g/executables/nr-softmodem.c) |
| SCHED_FIFO | 非 root 时 SCHED_FIFO 失败降级而非退出 | [system.c](file:///home/kongbai/openairinterface5g/common/utils/system.c) |

### ? 已解决的阻塞点：PDSCH 解调输出 LLR 全零（已修复）

**现象**：SIB1 PDU 长度正确（111 字节）但内容全零 → RRC 无法提取 PRACH 配置 → UE 不发起随机接入。

**根因**：RISC-V 架构下，[nr_dlsch_demodulation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_demodulation.c) 和 [nr_dlsch_llr_computation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_llr_computation.c) 中的 SIMD 代码被 `#if defined(__i386) || defined(__x86_64)` 条件跳过，导致 PHY 函数体为空，LLR 输出全零。

**修复方案**：在 29 处 `#if` 条件中添加 `|| defined(OAI_SIMD_X86_EMULATION)`，使 RISC-V 通过 SIMDE 库启用 x86 SSE2 仿真代码。

**验证**：修复后 SIB1 PDU 内容非零，RRC 成功提取 PRACH 配置，UE 发起随机接入，完整 RA + RRC + DRB + 数据传输链路跑通。

---

## 三、关键文件清单（含修改说明）

### A. 核心 PHY（当前调试重点）
| 文件 | 修改内容 | 为什么改 |
|------|---------|---------|
| [nr_dlsch_decoding.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_decoding.c) | LDPC `outMode` 修正回 `nrLDPC_outMode_BIT`(0)；添加 LLR 各阶段诊断日志（d[r]/z/l/LDPCoutput） | outMode=1 实际是 BITINT8 非 BIT，原始值 0 才正确 |
| [phy_procedures_nr_ue.c](file:///home/kongbai/openairinterface5g/openair1/SCHED_NR_UE/phy_procedures_nr_ue.c) | `p_b` 改为 `aligned_alloc` 堆分配；添加 PDU 内容 dump 日志 | `p_b` 存入 `pdsch_pdu.pdu` 供 MAC 在函数返回后访问，栈分配会悬垂 |
| [nrLDPC_decoder.c](file:///home/kongbai/openairinterface5g/openair1/PHY/CODING/nrLDPC_decoder/nrLDPC_decoder.c) | SIMDE 适配 | LDPC 译码器用大量 SIMD |

### B. SIMDE / SIMD 适配（PDSCH 全零嫌疑在这里）
| 文件 | 修改内容 |
|------|---------|
| [sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h) | x86 SIMD 宏重定向到 SIMDE |
| [simde/sse2.h](file:///home/kongbai/openairinterface5g/extra/simde/simde/x86/sse2.h) | SIMDE sse2 适配（**PDSCH 解调全零很可能在这里**）|

### C. 协议栈 / RRC / MAC
| 文件 | 修改内容 | 为什么改 |
|------|---------|---------|
| [rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_UE.c) | ASN.1 类型加 `struct` 关键字 | `NR_SystemInformation_IEs__sib_TypeAndInfo__Member` 缺 struct 关键字 |
| [rrc_timers_and_constants.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_timers_and_constants.c) | 移除 `ext1` 检查；`*rlf_tac->t311` 解引用 | asn1c 版本扁平化 ext1；t311 是指针需解引用才能 switch |
| [nr-softmodem.c](file:///home/kongbai/openairinterface5g/executables/nr-softmodem.c) | noS1 模式跳过 SCTP 任务创建 | 无 EPC 时 SCTP 初始化会失败 |
| [system.c](file:///home/kongbai/openairinterface5g/common/utils/system.c) | 非 root 时 SCHED_FIFO 失败降级 | 非 root 跑会 AssertFatal 退出 |

### C2. noS1 数据链路补全（2026-08-16/17 新增）
| 文件 | 修改内容 | 为什么改 |
|------|---------|---------|
| [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c) | noS1 模式手工触发 SecurityModeCommand；手工创建默认 DRB（drb_id、PDU session id=10、QFI=9）；将 DRB RLC bearer 加入 `masterCellGroup->rlc_BearerToAddModList`（必须在 `do_RRCReconfiguration` 之前） | noS1 模式无核心网，NGAP 被 stub，SecurityModeCommand 和 DRB 建立流程不会被正常触发 |
| [nr_pdcp_oai_api.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/nr_pdcp/nr_pdcp_oai_api.c) | `enb_tun_read_thread` 添加详细日志（TUN read、has_ue、sdap_data_req 返回值）；修复 QFI 不匹配（7→9）；`deliver_sdu_drb` 添加日志 | TUN 读取线程缺少日志难以调试；QFI 必须与 DRB 创建时的 QFI 一致 |
| [nr_sdap_entity.c](file:///home/kongbai/openairinterface5g/openair2/SDAP/nr_sdap/nr_sdap_entity.c) | noS1 模式下 gNB 上行数据直接写入 TUN 接口（`write(nas_sock_fd[0],...)`）替代发往 GTP-U；添加 `#include "executables/softmodem-common.h"` | noS1 模式无 GTP-U/UPF，上行数据发往 GTP-U 会被丢弃；需直接写入 gNB TUN 接口让 Linux 网络栈处理 |

### D. rfsim
| 文件 | 修改内容 | 为什么改 |
|------|---------|---------|
| [simulator.c](file:///home/kongbai/openairinterface5g/radio/rfsimulator/simulator.c) | epoll 错误非致命化；socketError 中 UE_MAGICDL 不 exit；flushInput 加 fcntl 检查 | UE 断开时 gNB 会崩溃退出 |

### E. 配置文件
| 文件 | 修改内容 |
|------|---------|
| [gnb.sa.band78.106prb.rfsim.conf](file:///home/kongbai/openairinterface5g/ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf) | `tr_s_preference="local_L1"`, `tr_n_preference="local_RRC"` |
| [nrue.band78.106prb.rfsim.conf](file:///home/kongbai/openairinterface5g/ci-scripts/conf_files/nrue.band78.106prb.rfsim.conf) | `phy_log_level="debug"` |
| [riscv64-toolchain.cmake](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-toolchain.cmake) | RISC-V 交叉编译工具链配置 |

### F. 协议 stub（39 个 .c.orig 备份）
E1AP/F1AP/M2AP/M3AP/S1AP 协议文件被替换为静态零初始化 stub，原始文件存为 `.c.orig`。清单见 `find . -name "*.c.orig"`。

---

## 四、构建环境

### 工具链
- 交叉编译器：`riscv64-linux-gnu-gcc 11.4.0`
- QEMU 用户态：`qemu-riscv64 6.2.0`（**已知 RVV 1.0 仿真有缺陷，但跑 OAI rfsim 约 17% 实时速度**）
- RISC-V sysroot：`/usr/riscv64-linux-gnu`
- 架构flags：`-march=rv64gcv -mabi=lp64d`

### RISC-V 库（`/home/kongbai/riscv-libs/`）
- `lib/` — 19 个 RISC-V 共享库（从 deb 包解压，含 soname 符号链接，含 `libgfortran.so.5`）
- `stubs_link.o` — 链接桩，解决 `__builtin_cpu_init`、`__builtin_cpu_supports`、`OPENSSL_assert`、`der_write_tags`、`der_encode`、`ber_decode`、`ber_check_tags` 等未定义符号
- `stubs.c` / `stubs_link.c` — 桩源码（有详细注释说明每个 stub 的作用）
- `zlib_stub.c` — zlib 桩
- `stub-backup/` — 旧版 stub 备份

### stubs 目录（`cmake_targets/riscv64-stubs/`）
- `CMakeLists.txt` — 构建 cblas/openssl stub
- `cblas_stubs.c`, `openssl_stubs.c` — BLAS/OpenSSL 桩
- `include/` — 本地头文件覆盖（覆盖 x86 系统头，如 `gnu/stubs.h`）

### cmake 初始化（从零构建 build-riscv 的命令）
```bash
cd /home/kongbai/openairinterface5g
mkdir build-riscv && cd build-riscv
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../cmake_targets/riscv64-toolchain.cmake \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo
# 然后链接桩需手动加入（cmake 不会自动加）：
#   编辑 CMakeFiles/nr-softmodem.dir/link.txt 和 nr-uesoftmodem.dir/link.txt
#   在链接命令中加入 /home/kongbai/riscv-libs/stubs_link.o
make nr-softmodem
make nr-uesoftmodem
make rfsimulator   # librfsimulator.so 不在默认目标里，必须单独编
```

### 编译 stubs_link.o（如需重新编译）
```bash
cd /home/kongbai/riscv-libs
riscv64-linux-gnu-gcc -c -march=rv64gcv -mabi=lp64d \
  -isystem /home/kongbai/openairinterface5g/cmake_targets/riscv64-stubs/include \
  -isystem /usr/riscv64-linux-gnu/include \
  stubs_link.c -o stubs_link.o
```

### 已知构建坑
1. **`libasn1_nr_rrc.a` 偶发 "No rule to make target"** → 手动跑 `bash build-riscv/openair2/RRC/NR/MESSAGES/CMakeFiles/asn1_nr_rrc.dir/link.txt`
2. **`ANY_aper.c` 生成失败** → 用空实现 stub + `touch` 防止重新生成
3. **重新 cmake 会丢失 link.txt 中的 stubs_link.o** → 必须重新手动添加
4. **librfsimulator.so 不在默认 make 目标** → 单独 `make rfsimulator`
5. **缺 libgfortran.so.5** → 已在 `riscv-libs/lib/` 补上，运行时需 `LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib`

---

## 五、运行命令（rfsim 接入测试）

### 0. 清理残留
```bash
sudo pkill -f nr-uesoftmodem; sudo pkill -f "qemu.*nr-softmodem"
sudo ip link delete oaitun_enb1 2>/dev/null
# 删 root 残留日志（否则非 root 跑会 Permission denied）
sudo rm -f /home/kongbai/openairinterface5g/build-riscv/nr*.log
```

### 1. 启动 gNB（必须 root，TUN 需要 CAP_NET_ADMIN）
```bash
cd /home/kongbai/openairinterface5g/build-riscv
sudo env LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib qemu-riscv64 \
  -L /usr/riscv64-linux-gnu ./nr-softmodem \
  -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf \
  --rfsim --sa --noS1 > /tmp/gnb_rfsim.log 2>&1 &
```
- 成功标志：`waiting opposite rfsimulators to connect`，端口 4043 监听
- 频率：`absoluteFrequencySSB` = 3319.68 MHz

### 2. 启动 UE（建议 root，SCHED_FIFO 对 RA 时序重要）
```bash
cd /home/kongbai/openairinterface5g/build-riscv
sudo env LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib qemu-riscv64 \
  -L /usr/riscv64-linux-gnu ./nr-uesoftmodem \
  -O ../ci-scripts/conf_files/nrue.band78.106prb.rfsim.conf \
  --rfsim --noS1 --sa -C 3319680000 > /tmp/ue_rfsim.log 2>&1 &
```
- **频率必须匹配 gNB**：`-C 3319680000`（3319.68 MHz）
- 成功标志：`pbch decoded sucessfully`、`init_sync_frame=0`、`State = NR_RRC_CONNECTED`

### 3. 等待 RRC + DRB 建立（约 60-90 秒）
```bash
# 检查 UE 状态
grep -iE "NR_RRC_CONNECTED|ReconfigurationComplete" /tmp/ue_rfsim.log
# 检查 gNB DRB
grep -iE "noS1.*DRB|added RLC bearer|ReconfigurationComplete" /tmp/gnb_rfsim.log
```

### 4. 设置路由（让数据走 5G 栈而非本地直连）
```bash
# 两个 TUN 在同一主机，需设置 ip rule 强制走 5G 栈
sudo ip route add 10.0.1.2 dev oaitun_enb1 table 10000  # 下行: 到 UE 的包走 gNB TUN
sudo ip route add 10.0.1.1 dev oaitun_ue1  table 10000  # 上行: 到 gNB 的包走 UE TUN
sudo ip rule add to 10.0.1.2 lookup 10000
sudo ip rule add to 10.0.1.1 lookup 10000
sudo ip rule add from 10.0.1.2 lookup 10000
sudo ip rule add from 10.0.1.1 lookup 10000
```

### 5. Ping 测试（端到端数据传输）
```bash
sudo ping -I 10.0.1.1 -c 3 10.0.1.2
# 应看到 0% packet loss

# 验证数据走 5G 栈（非本地路由）
grep "enb_tun_read: has_ue=1\|gNB TUN write\|UE TUN write" /tmp/gnb_rfsim.log /tmp/ue_rfsim.log
```

---

## 六、新账号接手第一步（自检 + 复现）

按顺序执行，确认环境 OK 并复现阻塞点：

```bash
# ① 环境自检
riscv64-linux-gnu-gcc --version                    # 应 11.4.0
qemu-riscv64 --version | head -1                   # 应 6.2.0
ls /home/kongbai/riscv-libs/stubs_link.o           # 必须存在
ls /home/kongbai/riscv-libs/lib/ | wc -l           # 应 ~19

# ② 代码改动自检
cd /home/kongbai/openairinterface5g
git status --short | wc -l                         # 应 ~170（未 commit）
grep -c stubs_link.o build-riscv/CMakeFiles/nr-uesoftmodem.dir/link.txt  # 应 1
grep -c stubs_link.o build-riscv/CMakeFiles/nr-softmodem.dir/link.txt    # 应 1

# ③ 二进制自检
ls -la build-riscv/nr-softmodem build-riscv/nr-uesoftmodem

# ④ 验证成功状态（见第五节运行命令）
# 启动 gNB + UE 后，应看到：
#   UE: pbch decoded sucessfully          ? PBCH 同步
#   UE: State = NR_RRC_CONNECTED          ? RRC 连接
#   gNB: noS1: created default DRB 1      ? DRB 建立
#   gNB: gNB TUN write (noS1): len=48    ? 上行数据
#   UE:  UE TUN write: len=48            ? 下行数据
```

---

## 七、任务完成状态（全部已完成 ?）

### ? 阶段 1：修复 PDSCH LLR 全零（已完成）
| # | 任务 | 入口文件 | 状态 |
|---|------|---------|------|
| 1 | 定位 LLR 全零根因：SIMD 代码被 `#if` 跳过 | [nr_dlsch_demodulation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_demodulation.c) | ? |
| 2 | 修复：29 处 `#if` 添加 `OAI_SIMD_X86_EMULATION` | 同上 + [nr_dlsch_llr_computation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_llr_computation.c) | ? |
| 3 | 验证 SIB1 PDU 非零，RRC 提取 PRACH 配置 | [rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_UE.c) | ? |

### ? 阶段 2：RA 流程（已完成）
| # | 任务 | 入口文件 | 状态 |
|---|------|---------|------|
| 4 | UE 发起 PRACH（Msg1） | [nr_ra_procedures.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_UE/nr_ra_procedures.c) | ? |
| 5 | gNB 回 Msg2（RAR） | gNB MAC 调度 | ? |
| 6 | UE 发 Msg3（RRC Setup Request） | UE MAC/PHY | ? |

**QEMU 17% 实时速度未影响 RA 时序**：rfsim 的时序比真实空口宽松，RA 流程顺利完成。

### ? 阶段 3：RRC 连接建立（已完成）
| # | 任务 | 入口文件 | 状态 |
|---|------|---------|------|
| 7 | gNB 回 Msg4（RRCSetup + 竞争解决） | gNB RRC | ? |
| 8 | UE 进入 RRC CONNECTED | [rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_UE.c) | ? |
| 9 | SecurityModeCommand + SecurityModeComplete | [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c) | ? |

**noS1 适配**：SecurityModeCommand 在 noS1 模式下由 rrc_gNB.c 手工触发（绕过 stubbed NGAP）。

### ? 阶段 4：DRB 建立与端到端数据传输（已完成）
| # | 任务 | 入口文件 | 状态 |
|---|------|---------|------|
| 10 | noS1 模式手工创建默认 DRB + RLC bearer | [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c) | ? |
| 11 | RRCReconfiguration → ReconfigurationComplete | 同上 | ? |
| 12 | 下行数据：gNB TUN→5G 栈→UE TUN（48 字节） | [nr_pdcp_oai_api.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/nr_pdcp/nr_pdcp_oai_api.c) | ? |
| 13 | 上行数据：UE TUN→5G 栈→gNB TUN（48 字节） | [nr_sdap_entity.c](file:///home/kongbai/openairinterface5g/openair2/SDAP/nr_sdap/nr_sdap_entity.c) | ? |
| 14 | Ping 双向验证通过（0% 丢包） | — | ? |

---

## 八、调试关键日志位置

| 日志 | 内容 |
|------|------|
| `/tmp/gnb_rfsim.log` | gNB 运行日志 |
| `/tmp/ue_rfsim.log` | UE 运行日志（含 LLR 诊断）|
| `build-riscv/nrL1_stats.log` | PHY L1 统计 |
| `build-riscv/nrMAC_stats.log` | MAC 统计 |
| `build-riscv/nrRRC_stats.log` | RRC 统计 |

### 关键诊断 grep
```bash
# UE 同步状态
grep -iE "pbch decoded|init_sync_frame|BCCH PDU bytes" /tmp/ue_rfsim.log
# LDPC 诊断（d[r]/z/l/LDPCoutput 各阶段值）
grep "LDPC debug" /tmp/ue_rfsim.log
# DCI / PDSCH
grep -iE "DLSCH Decoding|found.*DCI|DMRS|Channel estimation" /tmp/ue_rfsim.log
# gNB 连接状态
grep -iE "active_clients|waiting opposite" /tmp/gnb_rfsim.log
```

---

## 九、PDSCH LLR 全零排查指引（已解决，保留供回溯）

**已解决**：根因是 SIMD 代码被预处理器条件跳过，修复方案是在 `#if` 中添加 `OAI_SIMD_X86_EMULATION`。

**数据流**（已验证全部正常）：
```
DCI 解码 ? → DMRS 信道估计 ?(nb_rb=14) → 均衡 ? → 解调/LLR计算 ? → d[r] 非零 ? → LDPC 译码 ? → SIB1 PDU 非零 ?
```

**根因和修复**：
1. **入口**：[nr_dlsch_decoding.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_decoding.c) 约 255 行，`d[r]` 来自 `harq_process->d[r]`
2. **解调器**：[nr_dlsch_demodulation.c](file:///home/kongbai/openairinterface5g/openair1/PHY/NR_UE_TRANSPORT/nr_dlsch_demodulation.c)（QPSK 解调 + LLR 计算）
3. **根因**：SIMD 代码被 `#if defined(__i386) || defined(__x86_64)` 跳过，RISC-V 不匹配该条件
4. **修复**：在 29 处 `#if` 添加 `|| defined(OAI_SIMD_X86_EMULATION)`，通过 SIMDE 启用 x86 SSE2 仿真

---

## 十、已解决问题的详细经过（供回溯）

### 1. ASN.1 编译错误
- **`NR_SystemInformation_IEs__sib_TypeAndInfo__Member` 未定义类型** → rrc_UE.c 加 `struct` 关键字
- **`NR_RLF_TimersAndConstants_t` 无 `ext1` 成员** → asn1c 版本扁平化了 ext1，移除检查
- **`switch` quantity not integer** → `rlf_tac->t311` 是指针，改 `*rlf_tac->t311` 解引用
- **`ANY_aper.c` 生成失败** → 空 stub 实现 + touch 防重新生成

### 2. 链接未定义符号
- `__builtin_cpu_init`/`__builtin_cpu_supports`（x86 专用 GCC 内建）→ stub 返回"无 x86 特性"（正确行为，OAI 会选可移植路径）
- `der_write_tags`/`der_encode`/`ber_decode`/`ber_check_tags`（asn1c BER/DER 运行时）→ stub 报失败（RRC 用 UPER 不用 BER/DER）
- `OPENSSL_assert` → no-op stub
- 全部在 `stubs_link.o` 中，手动加入 link.txt

### 3. rfsim 稳定性
- **gNB 在 UE 断开时崩溃** → `socketError()` 中 `exit(1)` 改为对 UE_MAGICDL 不退出
- **epoll_wait 断言失败** → 改为非致命日志 + return
- **EBADF（坏文件描述符）** → `flushInput()` 加 `fcntl` 有效性检查

### 4. gNB 启动问题
- **TUN 设备 busy** → `sudo ip link delete oaitun_enb1`
- **SCTP 初始化失败（noS1）** → nr-softmodem.c 跳过 SCTP 任务创建
- **`tr_s_preference`/`tr_n_preference` 错误** → 改为 `local_L1`/`local_RRC`
- **SIB1 未调度** → 加 `--sa` 参数

### 5. UE 同步问题
- **频率不匹配** → UE 用 3619.2 MHz，gNB 用 3319.68 MHz → UE 改 `-C 3319680000`
- **`first_sync_frame` 一直 -1** → PBCH 译码成功后解决

### 6. SIB1 PDU 全零（已修复）
- **LDPC outMode 错误** → 之前误改成 1(BITINT8)，实际应为 0(BIT) → 已修正回
- **p_b 栈分配悬垂** → 改 `aligned_alloc` 堆分配
- **根因**：`d[r]` 本身全零，因为 PDSCH 解调的 SIMD 代码被 `#if defined(__i386) || defined(__x86_64)` 跳过
- **修复**：在 29 处 `#if` 添加 `|| defined(OAI_SIMD_X86_EMULATION)`，RISC-V 通过 SIMDE 启用 x86 SSE2 代码

### 7. noS1 模式 SecurityModeCommand 不触发（已修复）
- **现象**：RRCSetupComplete 后 gNB 不发 SecurityModeCommand
- **根因**：noS1 模式下 NGAP 被 stub，正常由 AMF 触发的 SecurityModeCommand 流程不存在
- **修复**：在 [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c) 中检查 `IS_SOFTMODEM_NOS1`，手工触发 SecurityModeCommand

### 8. noS1 模式 DRB 不建立（已修复）
- **现象**：SecurityModeComplete 后无 RRCReconfiguration，DRB 不存在
- **根因**：noS1 模式无核心网发起 PDU session 建立
- **修复**：在 [rrc_gNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB.c) 中手工创建默认 DRB（PDU session id=10, QFI=9），并将 RLC bearer 加入 `masterCellGroup->rlc_BearerToAddModList`
- **关键坑**：RLC bearer 必须在 `do_RRCReconfiguration` 之前加入，否则不会被编码进发给 UE 的消息

### 9. gNB 上行数据丢失（已修复，2026-08-17）
- **现象**：UE 通过 DRB 发送的上行数据到达 gNB PDCP/SDAP 后丢失，未写入 gNB TUN
- **根因**：[nr_sdap_entity.c](file:///home/kongbai/openairinterface5g/openair2/SDAP/nr_sdap/nr_sdap_entity.c) 中 gNB 侧 `nr_sdap_rx_entity` 将上行数据发往 GTP-U（`itti_send_msg_to_task(TASK_GTPV1_U, ...)`），但 noS1 模式无 GTP-U/UPF，数据被丢弃
- **修复**：添加 `IS_SOFTMODEM_NOS1` 判断，noS1 模式下直接 `write(nas_sock_fd[0], ...)` 写入 gNB TUN 接口，让 Linux 网络栈处理
- **验证**：修复后日志显示 `gNB TUN write (noS1): len=48`，ping 双向通过 5G 栈验证成功

### 10. QFI 不匹配（已修复）
- **现象**：TUN 读取线程用 qfi=7，但 DRB 创建时用 qfi=9，SDAP 映射失败
- **修复**：将 `enb_tun_read_thread` 中 qfi 改为 9，与 DRB 创建时的 QFI 一致

---

## 十一、协议栈补全路线图（UE 接入完成后的远期目标）

当前 39 个协议文件被桩化为静态零初始化实现（原始代码存为 `.c.orig`），因为 rfsim noS1 模式下用不到核心网/CU-DU 分离/MBMS 接口。**远期目标是将这些协议栈全部补全为真实实现**。

### 桩化协议清单（39 个，按协议分组）

| 协议 | 文件数 | 目录 | 作用 | 为什么当初桩化 |
|------|--------|------|------|--------------|
| **S1AP** | 8 | `openair3/S1AP/` | eNB ? EPC（LTE）| noS1 无核心网 |
| **M3AP** | 14 | `openair3/M3AP/` | MCE ? MME（MBMS）| rfsim 不做多播 |
| **F1AP** | 8 | `openair2/F1AP/` | gNB-CU ? gNB-DU | 单体 gNB 不拆分 |
| **M2AP** | 5 | `openair2/M2AP/` | eNB ? MCE（MBMS）| rfsim 不做多播 |
| **E1AP** | 3 | `openair2/E1AP/` | CU-CP ? CU-UP | 单体 gNB 不拆分 |
| **NGAP** | 1 | `openair2/RRC/NR/rrc_gNB_NGAP.c.orig` | gNB ? 5GC | noS1 无核心网 |

完整文件清单：`find /home/kongbai/openairinterface5g -name "*.c.orig" | grep -v build`

### 补全方法（每个协议通用三步）

```bash
# 1. 恢复原始代码
mv xxx.c.orig xxx.c

# 2. 解决当初导致桩化的编译问题
#    （ASN.1 类型在 RISC-V 交叉编译下的报错——当初就是因为难修才桩化的）

# 3. 提供对端网元环境（真正的难点）
```

**难点不在恢复代码，而在第 2、3 步**：当初桩化就是因为这些协议的 ASN.1 代码在 RISC-V 交叉编译下编不过，且需要外部网元。

### 建议优先级（按价值/难度排序）

| 顺序 | 协议 | 价值 | 难度 | 需要的对端环境 |
|------|------|------|------|--------------|
| 1 | **NGAP** | ??? 连 5GC 是最常见需求 | 中 | 5GC 核心网（Open5GS / free5GC / OAI 5G Core）|
| 2 | **F1AP** | ?? CU/DU 分离是 5G 重要架构 | 高 | 拆成 gNB-CU 和 gNB-DU 两进程 |
| 3 | **S1AP** | ? LTE 兼容（5G 项目价值低）| 中 | EPC 核心网 |
| 4 | **E1AP** | ? CU-CP/UP 分离（较少用）| 高 | 进一步拆分 |
| 5 | **M2AP/M3AP** | ? MBMS 多播（5G 用得少）| 高 | MCE + MME |

**推荐先补 NGAP**：连真实 5GC 是最有价值的场景，且 Open5GS/free5GC 是现成开源 5GC，对端环境最容易搭。

### 工作量估计

- 单个协议"恢复代码 + 解决编译"：3-7 天（主要 ASN.1 RISC-V 交叉编译问题）
- 单个协议"真正跑通连对端"：1-3 周（搭核心网 + 调信令流程）
- **6 个全部补全：乐观 2-3 个月，悲观半年**

### 依赖关系

NGAP 补全后，`rrc_gNB_NGAP.c` 也要恢复，gNB 要从 noS1 切到 S1 模式连核心网——这会改变整体运行方式。补全顺序严格按上表，因为后面协议的对端环境往往依赖前面（如 M2AP 的 MME 就是 S1AP/NGAP 核心网的一部分）。

### 注意

- **当前阶段（UE 接入）不需要补全任何一个**，桩化状态完全够用
- 补全是 UE 接入完成后的远期目标，不要在 PDSCH LLR 全零问题解决前启动
- 补全会改变 gNB 运行模式（noS1 → S1），运行命令和配置都要相应调整

---

## 十二、交接注意事项

1. **git 未提交**：所有 170 个文件修改均未 commit。远程 `git@github.com:feeasolfnj/oai.git` 只有初始提交。**建议新账号先建分支提交**：
   ```bash
   cd /home/kongbai/openairinterface5g
   git checkout -b riscv-port
   git add -A && git commit -m "RISC-V port: SIMDE + ASN.1 + rfsim fixes"
   ```
   备份 patch：`/tmp/oai-riscv-changes.patch`（51026 行）

2. **build-riscv 4.8G**：不要删，重编译很慢；但重跑 cmake 会丢 `stubs_link.o`，需重新加到 link.txt

3. **sudo 密码**：`123123`（gNB 必须 root 跑 TUN 设备）

4. **记忆系统**：`/home/kongbai/.trae-cn/memory/projects/.../project_memory.md` 有项目级约束，新账号可读取

5. **QEMU 速度**：约 17% 实时速度，RA 时序可能紧张，观察是否需要放宽 UE 侧定时器

6. **关键约束**（务必遵守）：
   - LDPC `outMode` 必须是 `nrLDPC_outMode_BIT`(0)，不是 1
   - `p_b` 必须 `aligned_alloc` 堆分配
   - gNB 必须 root 运行
   - UE 频率必须 `-C 3319680000`
   - 协议 stub 必须返回静态零初始化结构，不能返回 NULL
   - RISC-V 构建必须定义 `OAI_SIMD_X86_EMULATION` 宏
   - x86 SIMD 代码的预处理器条件必须包含 `|| defined(OAI_SIMD_X86_EMULATION)`
   - noS1 模式 ping 测试前必须设置 ip rule + table 10000 路由（见第五节第 4 步），否则数据走本地路由不经 5G 栈
   - DRB 的 RLC bearer 必须在 `do_RRCReconfiguration` 之前加入 `masterCellGroup->rlc_BearerToAddModList`
   - `enb_tun_read_thread` 的 QFI 必须与 DRB 创建时的 QFI 一致（均为 9）
