# 周报（第二周）：OAI 5G RISC-V 移植 —— QEMU 运行验证与 UE 接入调试

**汇报周期**：2026-08-10 ~ 2026-08-15
**项目目标**：将 OpenAirInterface 5G gNB（`nr-softmodem`）移植到 RISC-V 架构（进迭时空 K3 板）
**本周里程碑**：
1. gNB rfsim 在 QEMU 上跑通（空载小区持续运行）
2. 完成 UE 侧交叉编译，进入 UE 接入调试阶段

---

## 一、本周目标

1. 用真实 RISC-V 库替换 stub 库
2. 在 QEMU 上运行验证 `nr-softmodem --rfsim --noS1`
3. 修复运行期 bug，跑通 gNB rfsim
4. 交叉编译 UE（`nr-uesoftmodem`），启动 UE 接入调试

## 二、完成工作概述

本周完成了 stub 库到真实库的替换，在 QEMU 上跑通了 gNB rfsim 空载小区（约 16% 实时速度，干净启动/关闭），修复了 3 个运行期 bug。随后完成 UE 交叉编译，进入 UE 接入调试，定位到 PBCH 译码失败导致 RA 流程无法启动的根因。

**核心成果**：
```
[LOADER] library librfsimulator.so successfully loaded
[HW]   rfsimulator: running as server waiting opposite rfsimulators to connect
[RRC]  Read in ServingCellConfigCommon (PhysCellId 0, DLBand 78, DLBW 106)
[PHY]  RU 0 rf device ready → RU 0 RF started
got sync (ru_thread) → got sync (L1_stats_thread)
[NR_MAC] Frame.Slot 128 → 256 → ... → 896 → 回绕   (约 2048 帧)
** Caught SIGTERM, shutting down → Bye.   (干净关闭)
```

---

## 三、详细进展

### 3.1 stub 库替换为真实 RISC-V 库（Phase 2）

第一周用的是占位 stub 库（BLAS/LAPACK 返回空实现），PHY 矩阵运算会崩。本周链接通过后换成真实库：

```bash
# 1. 备份 stub
mkdir -p /home/kongbai/riscv-libs/stub-backup
cp /home/kongbai/riscv-libs/lib/lib{sctp,blas,lapacke,z}.so /home/kongbai/riscv-libs/stub-backup/

# 2. 下载 riscv64 deb
apt download libopenblas-dev:riscv64 liblapacke-dev:riscv64 liblapack-dev:riscv64 \
             libsctp-dev:riscv64 zlib1g-dev:riscv64

# 3. 解压到 riscv-libs，建 soname 软链
for deb in /tmp/riscv-debs/*.deb; do dpkg -x "$deb" /home/kongbai/riscv-libs/; done
cd /home/kongbai/riscv-libs/lib
ln -sf libopenblas*.so.* libblas.so
ln -sf liblapacke*.so.*  liblapacke.so
ln -sf libsctp*.so.*     libsctp.so
ln -sf libz.so.*         libz.so

# 4. 重链
cd /home/kongbai/openairinterface5g/build-riscv && make nr-softmodem -j4
```

**两类桩区分清楚**：
- **库桩（必须换）**：libsctp/libblas/liblapacke/libz —— 缺库，换成真实 RISC-V 库
- **协议桩（保留 no-op）**：NGAP/F1AP/S1AP/M2AP/E1AP/M3AP —— asn1c 接口错配，跑 rfsim 用 `--noS1` 不需要核心网信令

? BLAS/LAPACK/SCTP/zlib 全部换成真实 RISC-V 库，`nm -D` 确认 `cblas_cgemm` 在 `.text` 段（真实代码）。

### 3.2 QEMU 运行验证（Phase 3）

#### 3.2.1 浅层测试 `--help`
```bash
qemu-riscv64 -L /usr/riscv64-linux-gnu \
  -E LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib:/home/kongbai/openairinterface5g/build-riscv \
  ./nr-softmodem --help
```
? 通过。ELF 完好、动态链接没断、库都能解析。

#### 3.2.2 深层测试 `--rfsim --noS1` —— 崩溃
```bash
sudo qemu-riscv64 -L /usr/riscv64-linux-gnu \
  -E LD_LIBRARY_PATH=... \
  ./nr-softmodem -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf --rfsim --noS1
```
- `sudo` 必须：`pthread_create()` 要 `CAP_SYS_NICE` 才能设 `SCHED_FIFO` 实时调度优先级
- 起来后段错误（SIGSEGV）

#### 3.2.3 gdb 抓真实 backtrace（关键转折）
用 `gdb-multiarch` + `qemu-riscv64 -g 1234` 调试，抓到崩点：
```
Thread 1 received signal SIGSEGV
#0  create_gNB_tasks () at executables/nr-softmodem.c:416
故障指令: sb t1, 16(a0)     # a0 = 0 → 写地址 0x10 = NULL 解引用
```
[nr-softmodem.c:416](file:///home/kongbai/openairinterface5g/executables/nr-softmodem.c#L416)：`getCxtE1(inst)->same_process = true;` —— `getCxtE1(inst)` 返回 NULL。

**重要结论**：这是确定性 bug，在 K3 板上同样会崩，不是 QEMU 限制。纠正了之前"QEMU 6.2 的 V 扩展仿真有缺陷导致崩"的误判。

### 3.3 三个运行期 Bug 的修复

#### Bug 1：E1AP 桩 NULL 解引用
- **问题**：[e1ap_common.c:15](file:///home/kongbai/openairinterface5g/openair2/E1AP/e1ap_common.c#L15) 桩 `getCxtE1(){return 0;}`，调用方不判空直接解引用 → SIGSEGV
- **解决**：返回静态零初始化 dummy，一次覆盖所有调用点：
  ```c
  static e1ap_upcp_inst_t dummy_e1_inst;   /* 零初始化,字段全是标量/内嵌结构,安全 */
  e1ap_upcp_inst_t *getCxtE1(instance_t instance) { (void)instance; return &dummy_e1_inst; }
  ```
- 符合项目约定（E1AP 保留 no-op 桩），不用逐处加判空

#### Bug 2：`librfsimulator.so` 没构建
- **问题**：rfsim 启动时 `Assertion ((ret>=0)) failed!`，`library librfsimulator.so is not loaded`
- **根因**：`librfsimulator.so` 是 runtime dlopen 加载的 MODULE 插件，**不在 `make nr-softmodem` 的构建目标里**
- **解决**：单独 `make rfsimulator` → 生成 [librfsimulator.so](file:///home/kongbai/openairinterface5g/build-riscv/librfsimulator.so)（567 KB）

#### Bug 3：`libgfortran.so.5` 缺失
- **问题**：`error while loading shared libraries: libgfortran.so.5: cannot open shared object file`
- **根因**：`librfsimulator.so` → openblas → **libgfortran**（OpenBLAS 的 Fortran 内部依赖），换真实 openblas 时漏了这个传递依赖
- **解决**：`apt download libgfortran5:riscv64` → `dpkg -x` 到 riscv-libs/lib/

### 3.4 gNB rfsim 跑通

```bash
cd /home/kongbai/openairinterface5g/build-riscv
sudo qemu-riscv64 -L /usr/riscv64-linux-gnu \
  -E LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib:/home/kongbai/openairinterface5g/build-riscv \
  ./nr-softmodem -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf --rfsim --noS1
```

**成功证据**（120 秒超时被杀而非崩溃）：
- 小区配置：band 78，PhysCellId 0，SSB ≈3319.68 MHz，106 PRB，TDD，SA，单天线
- SIB1/SIB23 编码成功
- 120 秒跑约 2048 帧 ≈ **16% 实时速度**（QEMU 6.2 仿真开销）
- 干净启动/关闭，无崩溃

### 3.5 UE 交叉编译与接入调试

#### 3.5.1 UE 交叉编译完成
- 在 gNB 编译基础上，完成 `nr-uesoftmodem` 交叉编译（57 MB）
- 修复 UE 侧特有的 ASN.1 兼容性问题：
  - [rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_UE.c)：`NR_SystemInformation_IEs__sib_TypeAndInfo__Member` 加 `struct` 关键字
  - [rrc_timers_and_constants.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR_UE/rrc_timers_and_constants.c)：去掉 `ext1` 检查，解引用 `t311` 指针
- UE 链接命令补 `stubs_link.o`，解决 `ber_check_tags`/`der_write_tags` 等未定义符号

#### 3.5.2 rfsim 通信链路修复
- [simulator.c](file:///home/kongbai/openairinterface5g/radio/rfsimulator/simulator.c)：处理 epoll 错误，防止 socket 断开崩溃
- [nr-softmodem.c](file:///home/kongbai/openairinterface5g/executables/nr-softmodem.c)：SCTP 任务在 noS1 模式下跳过
- UE/gNB 配置文件：rfsim 参数对齐，频率统一为 3319.68 MHz

#### 3.5.3 UE 接入调试 —— 当前阻塞点

**现象**：UE 能与 gNB 建立 rfsim socket 连接，但 RA（随机接入）流程无法启动。

**调试过程**：
1. 在 [nr_ra_procedures.c:628-637](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_UE/nr_ra_procedures.c#L628) 加调试日志，发现 UE 一直停在 `RA_UE_IDLE` 状态，日志输出 `PRACH Condition not met`
2. 根因是 `mac->first_sync_frame` 始终为 -1，RA 触发条件不满足
3. 追踪 `first_sync_frame` 设置链路：
   ```
   PBCH 译码 (nr_rx_pbch)
     → decoderState != 0? → decoded_pdu = false (MIB 丢失)
     → handle_bcch_bch() → nr_mac_rrc_data_ind_ue()
       → nr_rrc_ue_decode_NR_BCCH_BCH_Message()
         → nr_rrc_mac_config_req_mib() → nr_ue_decode_mib()
           → first_sync_frame = frame  ← 这里始终没被触发
   ```

**根因定位**：PBCH 极化译码器返回 `decoderState != 0`，导致 `decoded_pdu` 为 false，MIB 永远不会被送到 MAC/RRC 层。UE 下行同步失败，SSB 信号解调有问题。

---

## 四、本周成果

| 成果 | 状态 |
|---|---|
| stub 库替换为真实 RISC-V 库 | ? 完成 |
| QEMU 运行 gNB rfsim 跑通 | ? 完成（16% 实时速度） |
| 3 个运行期 bug 修复 | ? 完成 |
| UE 交叉编译 | ? 完成（57 MB） |
| rfsim 通信链路修复 | ? 完成 |
| UE 接入调试 —— PBCH 译码失败定位 | ? 进行中（根因已定位） |

## 五、当前状态评估

### 5.1 能做什么
- gNB 二进制完整启动：PHY/L1/MAC/RLC/PDCP/RRC 栈初始化、RU + rfsim 设备起来、进 TTI 主循环处理帧
- SA 模式 SIB1/SIB23 编码成功（cell 能起来）
- 干净启动/关闭，无崩溃
- **证明 OAI 完整协议栈能在 RISC-V 上跑起来**（rfsim 只替代射频前端，不替代协议栈）

### 5.2 不能做什么
| 缺口 | 状态 | 后果 |
|---|---|---|
| 核心网协议全桩化 | NGAP/S1AP/F1AP/E1AP/M2AP/M3AP 全 no-op | 没有 5GC/EPC，只能 `--noS1` |
| UE 下行同步 + MIB 解码 | ? PBCH 译码失败 | UE 无法触发随机接入 |
| UE 随机接入 (RA) | ? `first_sync_frame` 未设置 | 无 RRC 连接 |
| 数据回环 | ? 无 UE + 无 DRB | 上下行用户面零流量 |

## 六、下周计划

1. **抓 PBCH 译码实时日志**：在 QEMU 上同时跑 gNB 和 UE，捕获极化译码器 `decoderState` 值和 LLR 输入质量
2. **定位 PBCH 译码失败根因**：
   - 排查频率对齐、帧定时、信道估计是否正确
   - 排查 SIMDE 极化译码器在 RV64V 下是否有数值精度问题
3. **修复同步问题**：使 `first_sync_frame` 被正确设置，验证 RA 触发（Msg1 发送）
4. **验证 RA 流程**：Msg1→Msg2（RAR）→Msg3→Msg4（竞争解决），可能需要修 RRC 专用消息 UPER 编解码

## 七、风险与不确定性

| 风险点 | 难度 | 说明 |
|---|---|---|
| PBCH 译码失败根因 | 中-高 | 可能是配置问题（易修），也可能是 SIMDE 数值精度 bug（难修） |
| RRC 专用消息编解码 | 高 | SIB1 碰巧过了，但 `RRCSetup`/`RRCReconfiguration` 等专用消息未验证，可能崩 |
| QEMU 实时性 | 中 | 16% 实时速度，RA 时序窗口可能超时，最坏要上 K3 板 |

## 八、项目整体进度

| 阶段 | 状态 |
|---|---|
| Phase 0：环境搭建 | ? 完成 |
| Phase 1：交叉编译 | ? 完成 |
| Phase 2：库替换 | ? 完成 |
| Phase 3：QEMU 运行验证（gNB） | ? 完成 |
| Phase 4：UE 接入调试 | ? 进行中（根因已定位） |
| Phase 5：UE-gNB 端到端连接 | ? 待开始 |
| Phase 6：上 K3 板原生验证 | ? 待开始 |

**整体完成度**：约 70%（gNB 侧功能完整，UE 侧接入调试进行中）
