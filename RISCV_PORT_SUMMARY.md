# OAI 5G RISC-V 移植 —— 完整项目记录（从零到 rfsim 跑通）

> 本文档记录 OpenAirInterface 5G 移植到 RISC-V 架构（进迭时空 K3 板）的**全部工作**：环境搭建、交叉编译、链接、库替换、QEMU 运行验证，以及过程中遇到的**所有困难和解决方法**。横跨多个开发账号，按时间顺序组织。
>
> **最终成果**：在 x86 主机交叉编译出 RISC-V `nr-softmodem` ELF，并在 QEMU 上跑通 gNB rfsim `--noS1`（栈完整初始化 + 进 TTI 主循环处理帧 + 干净关闭，无崩溃）。

---

## 0. 项目概述

| 项 | 值 |
|---|---|
| 目标 | 把 OAI 5G gNB (`nr-softmodem`) 移植到 RISC-V，进迭时空 K3 板 |
| 原则 | 不考虑性能，只要能跑起来 |
| 主机 | Linux 22.04 Ubuntu (x86_64) |
| 交叉编译器 | `riscv64-linux-gnu-gcc` (gcc-cross 11) |
| 编译 flags | `-march=rv64gcv -mabi=lp64d` |
| SIMD 方案 | SIMDE 库模拟 x86 SSE/AVX → RV64V |
| 阶段一 | x86 主机交叉编译出 RISC-V ELF ? |
| 阶段二 | stub 库替换为真实 RISC-V 库 ? |
| 阶段三 | QEMU 运行验证 rfsim ?（空载小区） |

---

## 1. 环境搭建（Phase 0）

这是最早的工作，解决"RISC-V 上根本没有 OAI 的构建基础"这个问题。

### 1.1 交叉编译工具链

| 组件 | 文件 | 作用 |
|---|---|---|
| 工具链文件 | [cmake_targets/riscv64-toolchain.cmake](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-toolchain.cmake) | 指定交叉编译器、AR、stubs include 优先级 |
| gcc 包装器 | [cmake_targets/riscv64-gcc-wrapper.sh](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-gcc-wrapper.sh) | 过滤 CMake 误加的 `-I/usr/include/x86_64-linux-gnu` |
| OpenSSL 桩 | [cmake_targets/riscv64-stubs/](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-stubs) | `openssl_stubs.c` + 本地头覆盖（`gnu/stubs.h` 等缺失头） |

### 1.2 SIMD 模拟层（核心创新）

OAI 的 PHY 层大量使用 x86 SSE/AVX intrinsic（`_mm_*`、`__m128i`），RISC-V 没有这些。解决思路：

- 引入 **SIMDE 库**，把 x86 intrinsic 调用翻译成 RISC-V V 扩展（RV64V）
- 大改 [openair1/PHY/sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h)：
  - 定义 `OAI_SIMD_X86_EMULATION` 宏
  - 定义 `__x86_64__`（让现有 x86 代码路径被选中）
  - 做 `__m128i` → `simde__m128i` 类型映射
  - 开 `SIMDE_ENABLE_NATIVE_ALIASES`（让 `_mm_*` 解析到 SIMDE 模拟实现）
- 修 [extra/simde/simde/x86/sse2.h](file:///home/kongbai/openairinterface5g/extra/simde/simde/x86/sse2.h) 的 `_mm_pause` 兼容性
- **关键约束**：不定义 `SIMDE_X86_*_NATIVE`（那会让 SIMDE 去包含 `immintrin.h` 等 x86 头，根本不存在）

### 1.3 asn1c 工具链

- 使用 OAI 官方推荐版本：`mouse07410/asn1c` 的 `vlm_master` 分支
- 包装器 [/home/kongbai/bin/asn1c-oai](file:///home/kongbai/bin/asn1c-oai)：做了大量后处理（文件名前缀、类型名映射、时间戳跳过检查）
- **关键约束**：不要用系统 `/usr/bin/asn1c` 直接替代包装器

### 1.4 RISC-V 库依赖

`libconfig` 在主机上只有 x86 版本，交叉编译找不到 `-lconfig`。解决办法：从 Ubuntu ports 下载 riscv64 的 deb 包解压到 [/home/kongbai/riscv-libs/](file:///home/kongbai/riscv-libs)，CMake 加路径：

```cmake
if(${CPUARCH} STREQUAL "riscv64" AND EXISTS "/home/kongbai/riscv-libs")
  include_directories("/home/kongbai/riscv-libs/include")
  link_directories("/home/kongbai/riscv-libs/lib")
endif()
```

---

## 2. 阶段一：交叉编译出 ELF

编译过程经历了**三波编译错误 + 链接阶段**，每一波都是不同的根因。

### 2.1 第一波：环境/工具链/ASN.1 兼容层（10 个问题）

这是最早遇到的一批，主要是"RISC-V 上缺这缺那"和"asn1c 生成代码与 OAI 源码接口错配"。

| # | 问题 | 根因 | 解决方法 |
|---|---|---|---|
| 1 | libconfig 交叉编译找不到 | 主机只有 x86 版 | 下载 riscv64 deb 解压到 riscv-libs |
| 2 | `X2AP_InitiatingNodeType_EndcX2Setup_t` 未知类型 | 兼容头缺 typedef | `X2AP_oai_compat.h` 补 typedef struct |
| 3 | `asn1cSeqAdd` 宏参数数不匹配 | OAI 用 2 参，宏定义 3 参 | `oai_asn1.h`/compat 头重定义宏支持 2 参 |
| 4 | X2AP 头文件循环 include | include 顺序问题 | 重排 include 顺序，union 定义前移 |
| 5 | `X2AP_EUTRA_Mode_Info_t` 重定义 | compat 头与生成头重复 | compat 头删掉重复定义 |
| 6 | NGAP/X2AP 生成代码用 `ANY_t`，OAI 用 union | asn1c open-type 生成 `ANY_t`（无 `.present/.choice`） | 改 ASN.1 + 改生成头 + compat 头 |
| 7 | SIMDE `_mm_pause` 在 RISC-V 不可用 | x86 pause 指令无对应 | sse2.h 修 |
| 8 | `nr_mac_common.c` 的 `ext1` / 枚举命名 | ASN.1 扩展成员拍平 | 改源码访问方式 |
| 9 | CMake 误加 x86 多架构 include 路径 | CMake 默认行为 | gcc 包装器 + toolchain `-isystem` 优先级 |
| 10 | OpenSSL / `gnu/stubs.h` 等 RISC-V 缺失头 | RISC-V sysroot 不完整 | riscv64-stubs 桩库 |

**辅助脚本**（生成兼容头，放在 [riscv-port-scripts/](file:///home/kongbai/openairinterface5g/riscv-port-scripts)）：
- `fix_x2ap_compat_v2.py` —— 扫描 X2AP 源码生成 `X2AP_oai_compat.h`
- `fix_all_constants.py` / `fix_x2ap_constants.py` —— 补全 `*_PR_*` 枚举常量
- `fix_ngap_*.py` —— NGAP 编码器/兼容头修复
- `fix_nr_mac_common.py` —— nr_mac_common.c 修复

### 2.2 第二波：SIMD 类型丢失 + 源码 ext1 扁平化 + uper 参数

续编从 31% LDPC 段往后推进时遇到的一批。

#### 2.2.1 `vect128` / `mulByConjugate128` 未定义（SIMD 类型丢失）

- **现象**：`nr_pbch.c:254` 报 `unknown type name 'vect128'`、`'rxdataF128' undeclared`
- **根因**：原版 `sse_intrin.h` 有三路 `vect128` 定义（x86/arm/SIMDE）+ `mulByConjugate128` 实现，但 RISC-V 移植大改该文件时**整段删掉了**。`nr_pbch.c` 两者都用。
- **解决**：在 [sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h) 末尾补回 `vect128` + `minusConjug128` + `mulByConjugate128` 块。在 `OAI_SIMD_X86_EMULATION` 策略下 `__x86_64__` 已定义，x86 分支被选中，逻辑自洽。
- **代价**：改 `sse_intrin.h`（被几乎所有 PHY/LDPC 文件 include）触发了整个 LDPC+PHY 层重编，LDPC 译码器 5 个变体单个 `.o` 达 99MB，是 15-30 分钟慢段。

#### 2.2.2 `cblas.h` 找不到

- **现象**：`linear_preprocessing_rec.c:14: fatal error: cblas.h: No such file or directory`
- **根因**：BLAS 头 x86 主机有但 RISC-V 交叉编译路径里没有
- **解决**：按 `riscv64-stubs/` 模式新增 `cblas.h` 头 + `cblas_stubs.c` 实现，加进 `ssl_stubs` 库（已被链接进 nr-softmodem）

#### 2.2.3 `ext1` 扩展成员访问（多文件，反复出现）

- **现象**：`NR_TDD_UL_DL_Pattern_t has no member named 'ext1'` 等，出现在 `config.c`、`config_ue.c`、`gNB_scheduler_dlsch.c`、`nr_ue_scheduler.c`、`pdcp.c`、`rrc_UE.c`、`rrc_eNB.c`、`rrc_sl_preconfig.c` 等多个文件
- **根因**：asn1c 生成的结构体把 ASN.1 扩展成员（r9/r10/r12/r13/r14/r15）**拍平成直接成员**，去掉了 `ext1`/`ext2`/`ext4` 包装层。但 OAI 源码仍用 `->ext1->member` 间接访问。
- **解决原则**：**先核对生成结构体实际成员再改，不盲删**。去掉 `->ext1->`/`->ext4->` 间接层，直接访问拍平后的成员。
- **踩坑**：有的结构体有 extN 成员（如 `NR_PDCP_Config` 有 ext1 能编译），有的没有（如 `NR_TDD_UL_DL_Pattern`、`SL_PreconfigSync_r12` 没有）。不能批量盲删，必须逐个核对。

#### 2.2.4 `uper_encode_to_buffer` 参数数不匹配

- **现象**：OAI 源码用 5 参调用，asn1c 生成的函数是 4 参
- **解决**：用 Python 脚本统一修所有 5 参 → 4 参（删 NULL 第 2 参），**50 处替换跨 15 个文件**

#### 2.2.5 `setup` 泛型指针访问

- **现象**：`pdsch_ConfigCommon->choice.setup->MEMBER` 报 "not a structure or union"
- **根因**：ASN.1 改造后 `choice.setup` 成了泛型指针（`void*`），OAI 的 `->setup->member` 失效
- **解决**：强转到正确类型，如 `((NR_PDSCH_ConfigCommon_t*)...->choice.setup)->member`

#### 2.2.6 NR RRC 头文件循环 include

- **现象**：`NR_DMRS-UplinkConfig.h` 的 `#include "NR_SetupRelease.h"`（5234 行巨型聚合头）在 typedef 之前被处理，触发循环依赖
- **解决**：在 struct 前加前置 typedef，把 SetupRelease 的 include 移到 struct 之后

#### 2.2.7 NB_IoT 模块的裸 include

- **现象**：`NB_IoT_config.c:33: fatal error: log.h: No such file`，后续还有不存在的 `SystemInformationBlockType2.h`
- **关键发现**：**NB_IoT 这个 LTE-eNB 模块根本不是 nr-softmodem 的依赖**（不在 link.txt、不在 add_dependencies）
- **解决**：停掉 `make all`，改用 `make nr-softmodem`——只构建目标及依赖，自动跳过 NB_IoT、lte-softmodem 等无关目标。**这是减少无关修复的关键决策。**

#### 2.2.8 X2AP 兼容头 v2 脚本的 bug

- **现象**：`X2AP_X2SetupRequest_IEs_t` 等 typedef 虽然 v2 脚本生成了，但编译器说 unknown
- **根因**：v2 脚本**捏造**了 `struct X2AP_ProtocolIE_Field`（用了不存在的 `X2AP_IE_Value_t`）导致 struct 定义失败，后续 typedef 全部失效；还缺少 `*/` 注释终止符导致代码被错误注释
- **解决**：手动修正兼容头，修脚本

### 2.3 第三波：61 个编译错误（2026-08-09，6 类）

集中爆发在 NGAP/NR RRC 区域，按交接文档 11.1 节分类：

| 类别 | 错误数 | 文件 | 修法 |
|---|---|---|---|
| ① NGAP 类型/常量缺失 | 30 | `rrc_gNB_NGAP.c` | **整个文件打成 stub**（原文件备份 `.c.orig`），11 个函数全部 no-op |
| ② NR SetupRelease 不完整类型 | 18 | `nr_rrc_config.c`(9) + `gnb_config.c`(9) | 在 `NR_SetupRelease.h` 末尾 aliases 节插入 184 个 `#define`，把 OAI 期望的 struct tag 映射到 asn1c 生成的 `SetupRelease_2173PXX` |
| ③ `ext1`/`ext4` 拍平扩展 | 7 | `rrc_gNB_radio_bearers.c`(6) + `eNB_scheduler_primitives.c`(1) | 去掉 `->ext1->`/`->ext4->` 间接层 |
| ④ M3AP stub .h 清理 | 6 | `m3ap_{MCE,MME}_interface_management.{h,c}` | 引用缺失类型的函数参数改为 `void *` |
| ⑤ gnb_config 混杂 | 9 | `gnb_config.c` | SetupRelease(同②) + `struct` 关键字 + `NR_` 前缀 + 去 `F1AP_` 前缀 |
| ⑥ rrc_vars.h ext1 拍平 | 5 | `rrc_vars.h` | 去掉 `struct LTE_LogicalChannelConfig__ext1` 包装 |

#### NR_SetupRelease.h 的 #define（最 tricky 的修复）

NR_SetupRelease 是 ASN.1 参数化类型 `SetupRelease<T>`，asn1c 生成 `SetupRelease_2173PXX`，但 OAI 源码用 `NR_SetupRelease_PUSCH_Config` 等名字。关键细节：

- **struct tag**：`SetupRelease_2173P26`（**不带 NR_ 前缀**）
- **typedef**：`NR_SetupRelease_2173P26_t`（**带 NR_ 前缀**）
- **enum**：`SetupRelease_2173P26_PR_setup`（**不带 NR_ 前缀**）

aliases 文件已有 typedef 和 enum 映射，**缺的是 struct tag 映射**。添加：
```c
#define NR_SetupRelease_PUSCH_Config SetupRelease_2173P26
```
**踩坑**：第一次错误写成 `#define ... NR_SetupRelease_2173P26`（多了 NR_），导致 struct 仍不完整。用 sed 批量修正。

#### rrc_gNB_NGAP.c stub 的注意事项

- 原文件 1354 行，11 个公开函数
- **不能 stub `rrc_gNB_modify_dedicatedRRCReconfiguration`**——它在 `rrc_gNB.c:798` 已有定义，重复定义会导致链接错误
- stub 函数都是 no-op，NGAP 信令功能全部不可用

### 2.4 链接阶段

编译 0 错误后进入链接，暴露三类问题。

#### 2.4.1 缺失库（stub 共享库）

RISC-V 交叉编译环境缺少 4 个库，用空实现 stub .so 替代：

| 库 | 提供的符号 | 说明 |
|---|---|---|
| `libsctp.so` | sctp_sendmsg, sctp_recvmsg, sctp_bindx 等 9 个 | 空实现返回 -1/0 |
| `libblas.so` | cblas_cgemm, cblas_zaxpy, cblas_zcopy 等 5 个 | 空实现 |
| `liblapacke.so` | LAPACKE_cgetrf, LAPACKE_cgetrs, LAPACKE_cheev | 空实现返回 0 |
| `libz.so` | deflate, inflate, gzopen, crc32 等 | 空实现 |

源文件：[/home/kongbai/riscv-libs/stubs.c](file:///home/kongbai/riscv-libs/stubs.c)

#### 2.4.2 重复定义

- **问题**：`multiple definition of 'rrc_gNB_modify_dedicatedRRCReconfiguration'`
- **根因**：该函数在 `rrc_gNB.c:798` 已有定义，NGAP stub 又定义了一次
- **解决**：从 `rrc_gNB_NGAP.c` stub 中删除该函数

#### 2.4.3 9 个未定义符号（本会话修复）

```
der_write_tags / ber_check_tags / der_encode / ber_decode / asn_imax2INTEGER
OPENSSL_assert
__builtin_cpu_init / __builtin_cpu_supports
s1ap_config
```

| 符号 | 来源 | 原因 |
|---|---|---|
| `der_write_tags` / `ber_check_tags` / `der_encode` / `ber_decode` / `asn_imax2INTEGER` | asn1c BER/DER runtime | OAI 自带 asn1c runtime 缺这几个；RRC 用 UPER 不走 BER/DER，但 `ANY.c`/`constr_SEQUENCE_OF.c` 等通用支持对象仍会拉入 |
| `OPENSSL_assert` | OpenSSL | `riscv64-stubs/include/openssl/macros.h` 把它宏成 `((void)0)`，但没 include 该头文件的 TU 会发出真实调用 |
| `__builtin_cpu_init` / `__builtin_cpu_supports` | x86 GCC builtin | RISC-V GCC 不提供；OAI 的 `log.c`/`cmult_sv.c` 用它选 x86 快速路径 |
| `s1ap_config` | S1AP 全局配置 | `s1ap_eNB.c` 被桩化丢了这个全局变量的定义 |

**解决**：在 [stubs.c](file:///home/kongbai/riscv-libs/stubs.c) 增加桩实现。关键细节是**返回 struct 的桩必须匹配 RV64 LP64D ABI**：

```c
/* asn_enc_rval_t: 24 字节 struct,RV64 LP64D 下通过隐藏 sret 指针(a0)返回 */
typedef struct { long encoded; void *failed_type; void *structure_ptr; } asn_enc_rval_stub_t;
asn_enc_rval_stub_t der_encode(void) {
    asn_enc_rval_stub_t r; r.encoded = -1; r.failed_type = 0; r.structure_ptr = 0; return r;
}
/* asn_dec_rval_t: 16 字节,通过 a0/a1 返回;code=RC_FAIL(2) 让调用方走错误路径 */
```

为避免桩共享库被真实库替换后丢失这 9 个符号，单独抽出 [stubs_link.c](file:///home/kongbai/riscv-libs/stubs_link.c) → `stubs_link.o` 静态链入。

**`__builtin_cpu_supports` 是 shim 不是纯桩**：返回 0（"不支持任何 x86 特性"）让 OAI 走可移植 C 路径，这正是 RISC-V 上的正确行为（SIMDE 单独处理 SIMD）。

#### 2.4.4 R_RISCV_JAL 重定位溢出

```
relocation truncated to fit: R_RISCV_JAL against `nr_ulsch_qam64_qam64'
```

- **根因**：`nr_ulsch_llr_computation.c` 里的 QAM 解调函数经 SIMDE 展开（x86 SIMD → RVV）后膨胀到 **1.63 MiB**，超过 RISC-V `JAL` 指令 ±1 MiB 跳转范围
- **解决**：对该文件单独用体积优化重编：
  ```bash
  riscv64-linux-gnu-gcc -march=rv64gcv -mabi=lp64d -Os -fno-unroll-loops \
    -c nr_ulsch_llr_computation.c -o /tmp/nr_ulsch_llr.o
  ```
  函数体积从 1.63 MiB 降到 704 KiB，符合 JAL 范围。把新 `.o` 替换进 `libPHY_NR.a` 后重链通过。

#### 2.4.5 link.txt 补 stubs_link.o

CMake 生成的 [link.txt](file:///home/kongbai/openairinterface5g/build-riscv/CMakeFiles/nr-softmodem.dir/link.txt) 默认不含 `stubs_link.o`，`make` 重链会重新报 9 个 undefined。手动在链接命令里加上 `stubs_link.o`。

> ?? CMake 重新 configure 会覆盖 `link.txt`，需重加。

#### 2.4.6 编译被中断（不是失败）

2026-08-09 账号积分耗尽，用户在 52% 时**手动中断**编译。job 显示 `failed/exit_code=-1` 是中断信号，`/tmp/build_v22.log` 末尾无 error。已编译的 `.o` 都在，续编会增量复用。

### 2.5 阶段一结果

```
nr-softmodem: ELF 64-bit LSB pie executable, UCB RISC-V, RVC, double-float ABI
Machine: RISC-V   Class: ELF64
```

? RISC-V `nr-softmodem` ELF 生成成功（121 MB，含 debug_info 未 strip）。

---

## 3. 阶段二：stub 库 → 真实 RISC-V 库

阶段一用的是占位 stub 库（BLAS/LAPACK 返回空实现），PHY 矩阵运算会崩。链接通过后换成真实库：

```bash
# 1. 备份 stub
mkdir -p /home/kongbai/riscv-libs/stub-backup
cp /home/kongbai/riscv-libs/lib/lib{sctp,blas,lapacke,z}.so /home/kongbai/riscv-libs/stub-backup/

# 2. 下载 riscv64 deb
apt download libopenblas-dev:riscv64 liblapacke-dev:riscv64 liblapack-dev:riscv64 \
             libsctp-dev:riscv64 zlib1g-dev:riscv64

# 3. 解压到 riscv-libs
for deb in /tmp/riscv-debs/*.deb; do dpkg -x "$deb" /home/kongbai/riscv-libs/; done

# 4. 建 soname 软链
cd /home/kongbai/riscv-libs/lib
ln -sf libopenblas*.so.* libblas.so
ln -sf liblapacke*.so.*  liblapacke.so
ln -sf libsctp*.so.*     libsctp.so
ln -sf libz.so.*         libz.so

# 5. 重链
cd /home/kongbai/openairinterface5g/build-riscv && make nr-softmodem -j4
```

**两类桩要区分**：
- **库桩（必须换）**：libsctp/libblas/liblapacke/libz —— 缺库，换成真实 RISC-V 库
- **协议桩（保留 no-op）**：NGAP/F1AP/S1AP/M2AP/E1AP/M3AP —— asn1c 接口错配，修不动；跑 rfsim 用 `--noS1` 不需要核心网信令

? BLAS/LAPACK/SCTP/zlib 全部换成真实 RISC-V 库，`nm -D` 确认 `cblas_cgemm` 在 `.text` 段（真实代码）。

---

## 4. 阶段三：QEMU 运行验证

### 4.1 浅层测试 `--help`

```bash
sudo apt install qemu-user
qemu-riscv64 -L /usr/riscv64-linux-gnu \
  -E LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib:/home/kongbai/openairinterface5g/build-riscv \
  ./nr-softmodem --help
```

? 通过。ELF 完好、动态链接没断、库都能解析。

### 4.2 深层测试 `--rfsim --noS1` —— 崩溃

```bash
sudo qemu-riscv64 -L /usr/riscv64-linux-gnu \
  -E LD_LIBRARY_PATH=... \
  ./nr-softmodem -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf --rfsim --noS1
```

- `sudo` 必须：`pthread_create()` 要 `CAP_SYS_NICE` 才能设 `SCHED_FIFO` 实时调度优先级
- 起来后段错误（SIGSEGV）

### 4.3 gdb 抓真实 backtrace（关键转折）

用 `gdb-multiarch` + `qemu-riscv64 -g 1234` 调试，抓到崩点：

```
Thread 1 received signal SIGSEGV
#0  create_gNB_tasks () at executables/nr-softmodem.c:416
故障指令: sb t1, 16(a0)     # a0 = 0 → 写地址 0x10 = NULL 解引用
```

[nr-softmodem.c:416](file:///home/kongbai/openairinterface5g/executables/nr-softmodem.c#L416)：
```c
getCxtE1(inst)->same_process = true;   // getCxtE1(inst) 返回 NULL
```

### 4.4 纠正两个误判

| 之前的推测 | gdb 证伪 |
|---|---|
| "QEMU 6.2 的 V 扩展仿真有缺陷导致崩" | ? 错。崩点不在 PHY/矩阵运算，在 main 线程的 E1AP 上下文获取 |
| "崩在 SIB 编码（日志最后是 `Checking release`）" | ? 错。那行是 RRC 子线程的异步日志，恰好在崩溃前最后打印，不是崩点 |

**重要结论**：这是确定性 bug，在 K3 板上同样会崩，不是 QEMU 限制。

**gdb 调试技巧**：OAI 用 SIGUSR1/SIGUSR2(ITTI)/SIGALRM，gdb 里要 `handle SIGUSR1/2 SIGALRM SIGPIPE nostop noprint pass`。**不要在命令开头用 `pkill -f 'qemu-riscv64 -g 1234'`**——模式会匹配当前 shell 自己的命令行从而自杀。

---

## 5. 三个运行期 Bug 的修复

### 5.1 Bug 1：E1AP 桩 NULL 解引用

- **问题**：[e1ap_common.c:15](file:///home/kongbai/openairinterface5g/openair2/E1AP/e1ap_common.c#L15) 桩 `getCxtE1(){return 0;}`，调用方 [nr-softmodem.c:416](file:///home/kongbai/openairinterface5g/executables/nr-softmodem.c#L416) 不判空直接解引用 → SIGSEGV
- **解决**：返回静态零初始化 dummy，一次覆盖所有调用点：
  ```c
  static e1ap_upcp_inst_t dummy_e1_inst;   /* 零初始化,字段全是标量/内嵌结构,安全 */
  e1ap_upcp_inst_t *getCxtE1(instance_t instance) { (void)instance; return &dummy_e1_inst; }
  ```
- 符合项目约定（E1AP 保留 no-op 桩），不用逐处加判空

### 5.2 Bug 2：`librfsimulator.so` 没构建

- **问题**：rfsim 启动时 `Assertion ((ret>=0)) failed!`，`library librfsimulator.so is not loaded`
- **根因**：`librfsimulator.so` 是 runtime dlopen 加载的 MODULE 插件，**不在 `make nr-softmodem` 的构建目标里**
- **解决**：单独 `make rfsimulator` → 生成 [librfsimulator.so](file:///home/kongbai/openairinterface5g/build-riscv/librfsimulator.so) (567 KB)

### 5.3 Bug 3：`libgfortran.so.5` 缺失

- **问题**：`error while loading shared libraries: libgfortran.so.5: cannot open shared object file`
- **根因**：`librfsimulator.so` → openblas → **libgfortran**（OpenBLAS 的 Fortran 内部依赖），阶段二换真实 openblas 时漏了这个传递依赖
- **解决**：`apt download libgfortran5:riscv64` → `dpkg -x` 到 riscv-libs/lib/

---

## 6. 最终验证：rfsim 跑通

```bash
cd /home/kongbai/openairinterface5g/build-riscv
sudo qemu-riscv64 -L /usr/riscv64-linux-gnu \
  -E LD_LIBRARY_PATH=/home/kongbai/riscv-libs/lib:/home/kongbai/openairinterface5g/build-riscv \
  ./nr-softmodem -O ../ci-scripts/conf_files/gnb.sa.band78.106prb.rfsim.conf --rfsim --noS1
```

**成功证据**（120 秒超时被杀而非崩溃）：

```
[LOADER] library librfsimulator.so successfully loaded
[HW]   rfsimulator: running as server waiting opposite rfsimulators to connect
[RRC]  Read in ServingCellConfigCommon (PhysCellId 0, DLBand 78, DLBW 106)
[PHY]  RU 0 rf device ready → RU 0 RF started
got sync (ru_thread) → got sync (L1_stats_thread)
[NR_MAC] Frame.Slot 128 → 256 → ... → 896 → 回绕 → 896   (约 2048 帧)
** Caught SIGTERM, shutting down → Bye.   (干净关闭)
```

- 小区配置：band 78，PhysCellId 0，SSB ≈3319.68 MHz，106 PRB，TDD，SA，单天线
- SIB1/SIB23 编码成功
- 120 秒跑约 2048 帧 ≈ **16% 实时速度**（QEMU 6.2 仿真开销）

---

## 7. rfsim 跑出来的实际效果

**一个 band 78 的 gNB 小区起来了，但处于"空载对着虚空发射"的状态。**

- ? gNB 栈完整初始化（PHY/L1/MAC/RLC/PDCP/RRC/RU/rfsim 设备）
- ? SIB1/SIB23 编码成功，小区在"广播"系统信息
- ? TTI 调度器持续运行，`Frame.Slot` 计数推进
- ?? rfsim 是 server 模式等 UE 连，**没有 UE 客户端** → `No connected device, generating void samples`（RF 发空样点）
- ? 没有任何 UE 接入：grep `RNTI`/`RRCSetup`/`Msg1` 全为空
- ? 没有用户面数据：PDCP/RLC/SDAP/GTP 全空，SDAP 还被显式 `disabled`
- ? 没有数据回环

**本质**：一个"哑广播"小区——只在 SIB1 时隙发系统信息，其余 TTI 空转，RF 是空样点，上下行用户面零流量。

---

## 8. 功能完整性评估

### 8.1 能做什么

- gNB 二进制完整启动：PHY/L1/MAC/RLC/PDCP/RRC 栈初始化、RU + rfsim 设备起来、线程同步、进 TTI 主循环处理帧
- SA 模式 SIB1/SIB23 编码成功（cell 能起来）
- 干净启动/关闭，无崩溃
- **证明 OAI 完整协议栈能在 RISC-V 上跑起来**（rfsim 只替代射频前端，不替代协议栈，PHY/MAC/RLC/PDCP/RRC 都是真代码在执行）

### 8.2 不能做什么

| 缺口 | 状态 | 后果 |
|---|---|---|
| 核心网协议全桩化 | NGAP/S1AP/F1AP/E1AP/M2AP/M3AP 全 no-op | 没有 5GC/EPC，只能 `--noS1` |
| 没有 UE | rfsim server 等不到客户端 | 空跑空闲帧，无 RRC 连接、无业务 |
| RRC 专用消息编解码未验证 | asn1c 的 `ANY_t` vs union、ext 扁平化问题未修 | UE 一旦发起 RRC 连接很可能崩 |
| QEMU 跑不到实时 | ~16% 实时速度 | 端到端实时 5G 在 QEMU 上不行 |
| 无数据回环 | 无 UE + 无 DRB | 上下行用户面零流量 |

### 8.3 距离"功能完整"还差什么

1. **协议栈去桩**：修 asn1c 接口错配，恢复 NGAP/F1AP/E1AP 真实现（大头）
2. **接 UE**：跑 `nr-uesoftmodem --rfsim` 作为客户端连 gNB，验证空口同步 + RRC 连接建立 + DRB 建立
3. **接核心网**（SA 要 5GC）：否则 UE 注册/会话走不通
4. **上 K3 板**：QEMU 只能验证"能起来"，真跑业务得上原生实时平台

---

## 9. 全部修改文件清单

### 9.1 构建系统（CMake）
| 文件 | 改动 |
|---|---|
| [CMakeLists.txt](file:///home/kongbai/openairinterface5g/CMakeLists.txt) | 加 RISC-V 库路径、CPUARCH=riscv64 分支（SIMDE 模拟） |
| [cmake_targets/riscv64-toolchain.cmake](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-toolchain.cmake) | 交叉编译工具链 |
| [cmake_targets/riscv64-gcc-wrapper.sh](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-gcc-wrapper.sh) | 过滤 x86 多架构 include 路径 |
| [cmake_targets/riscv64-stubs/](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-stubs) | OpenSSL 桩 + 缺失头（含 cblas.h） |

### 9.2 SIMD 模拟层
| 文件 | 改动 |
|---|---|
| [extra/simde/simde/x86/sse2.h](file:///home/kongbai/openairinterface5g/extra/simde/simde/x86/sse2.h) | `_mm_pause` 兼容性 |
| [openair1/PHY/sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h) | 大改：`OAI_SIMD_X86_EMULATION`、`__m128i`→`simde__m128i` 映射、补回 `vect128`/`mulByConjugate128` |

### 9.3 协议层源码（ASN.1 兼容性，改动最多）
| 文件 | 改动 |
|---|---|
| [openair2/RRC/NR/rrc_gNB_NGAP.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB_NGAP.c) | 整文件 stub（原文件 `.c.orig`） |
| [openair2/RRC/NR/rrc_gNB_radio_bearers.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB_radio_bearers.c) | ext1 扁平化 |
| [openair2/RRC/NR/MESSAGES/asn1_msg.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/MESSAGES/asn1_msg.c) | NR RRC 编解码兼容 |
| [openair2/RRC/LTE/MESSAGES/asn1_msg.c](file:///home/kongbai/openairinterface5g/openair2/RRC/LTE/MESSAGES/asn1_msg.c) | RRC 编解码兼容 |
| [openair2/LAYER2/NR_MAC_COMMON/nr_mac_common.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_COMMON/nr_mac_common.c) | ext1/枚举命名 |
| [openair2/LAYER2/NR_MAC_gNB/config.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_gNB/config.c) | ext1 扁平化 + uper 4 参 |
| [openair2/LAYER2/NR_MAC_UE/config_ue.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_UE/config_ue.c) | setup 强转 + ext1 |
| [openair2/LAYER2/MAC/eNB_scheduler_primitives.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/MAC/eNB_scheduler_primitives.c) | ext4 扁平化 |
| [openair2/LAYER2/NR_MAC_gNB/gNB_scheduler_dlsch.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_gNB/gNB_scheduler_dlsch.c) | ext1 + NR_SetupRelease + cast |
| [openair2/LAYER2/NR_MAC_UE/nr_ue_scheduler.c](file:///home/kongbai/openairinterface5g/openair2/LAYER2/NR_MAC_UE/nr_ue_scheduler.c) | 缺失常量 |
| [openair2/GNB_APP/gnb_config.c](file:///home/kongbai/openairinterface5g/openair2/GNB_APP/gnb_config.c) | SetupRelease + struct + NR_ 前缀 + F1AP_ 前缀 |
| [openair2/RRC/LTE/rrc_vars.h](file:///home/kongbai/openairinterface5g/openair2/RRC/LTE/rrc_vars.h) | ext1 扁平化 |
| [openair2/RRC/LTE/rrc_eNB.c](file:///home/kongbai/openairinterface5g/openair2/RRC/LTE/rrc_eNB.c) | extN 扁平化 |
| [openair2/RRC/LTE/rrc_UE.c](file:///home/kongbai/openairinterface5g/openair2/RRC/LTE/rrc_UE.c) | ext1/ext2 扁平化 |
| [openair2/RRC/LTE/rrc_sl_preconfig.c](file:///home/kongbai/openairinterface5g/openair2/RRC/LTE/rrc_sl_preconfig.c) | ext1 扁平化 |
| [openair2/PDCP/pdcp.c](file:///home/kongbai/openairinterface5g/openair2/PDCP/pdcp.c) | ext1/ext2 扁平化 |
| [openair2/E1AP/e1ap_common.c](file:///home/kongbai/openairinterface5g/openair2/E1AP/e1ap_common.c) | getCxtE1 返回 dummy（运行期 bug 修复） |
| [openair3/NGAP/](file:///home/kongbai/openairinterface5g/openair3/NGAP) | ngap_common.h + ngap_gNB*.c（8 文件）类型兼容 |
| [openair3/M3AP/](file:///home/kongbai/openairinterface5g/openair3/M3AP) | m3ap_*_interface_management.{h,c} 参数改 void* |
| [openair2/X2AP/x2ap_common.h](file:///home/kongbai/openairinterface5g/openair2/X2AP/x2ap_common.h) | X2AP 公共类型 |
| [openair2/COMMON/s1ap_messages_types.h](file:///home/kongbai/openairinterface5g/openair2/COMMON/s1ap_messages_types.h) | S1AP 消息类型 |
| `uper_encode_to_buffer` 5参→4参 | 50 处跨 15 个文件 |

### 9.4 ASN.1 源文件（让 asn1c 生成 union 而非 ANY_t）
- `openair2/RRC/NR/MESSAGES/ASN.1/nr-rrc-17.3.0.asn1`
- `openair2/E1AP/MESSAGES/ASN.1/38463-g80.R16.78.0.asn`
- `openair2/X2AP/MESSAGES/ASN1/R15/x2ap-15.{1.0,3.0,6.0}.asn1`
- `openair3/NGAP/MESSAGES/ASN1/ngap-15.8.0.asn1`
- `openair3/S1AP/MESSAGES/ASN1/R15/s1ap-15.{1.0,2.0,6.0}.asn1`
- `openair3/LPP/MESSAGES/ASN1/37355-g60.asn`

### 9.5 build-riscv 内生成产物（?? 不在 git，删 build-riscv 会丢）
- `X2AP_oai_compat.h` —— X2AP 兼容头（手写+脚本生成）
- `NGAP_oai_compat.h` —— NGAP 兼容头
- `NR_SetupRelease.h` —— 184 个 #define（struct tag 映射）
- `X2AP_InitiatingMessage.h` 等 —— `ANY_t value` 改成 union
- `NR_DMRS-UplinkConfig.h` —— include 顺序调整

### 9.6 新建文件
| 文件 | 用途 |
|---|---|
| [/home/kongbai/riscv-libs/stubs.c](file:///home/kongbai/riscv-libs/stubs.c) | 桩共享库源（4 个库 + 9 个链接符号） |
| [/home/kongbai/riscv-libs/stubs_link.c](file:///home/kongbai/riscv-libs/stubs_link.c) | 9 个链接符号独立 object |
| [/home/kongbai/riscv-libs/stubs_link.o](file:///home/kongbai/riscv-libs/stubs_link.o) | 静态链入 nr-softmodem |
| [/home/kongbai/riscv-libs/stub-backup/](file:///home/kongbai/riscv-libs/stub-backup) | 阶段二替换前的 stub 库备份 |
| [riscv-port-scripts/](file:///home/kongbai/openairinterface5g/riscv-port-scripts) | 辅助脚本（fix_*.py）+ saved-compat-headers/ 备份 |

### 9.7 真实 RISC-V 库（[/home/kongbai/riscv-libs/lib/](file:///home/kongbai/riscv-libs/lib)）
- `libopenblas_riscv64_genericp-r0.3.20.so` → `libblas.so`
- `liblapacke.so.3` → `liblapacke.so`
- `libsctp.so.1` → `libsctp.so`
- `libz.so.1` → `libz.so`
- `libgfortran.so.5`（openblas 传递依赖）
- `libconfig.so`（最早期的库）

---

## 10. 关键约束与踩坑总结

### 10.1 必须遵守的约束
1. **不开 sanitizer**（全关）——RISC-V 交叉编译下 sanitizer 链接会出问题
2. **不用系统 `/usr/bin/asn1c`** 替代 `/home/kongbai/bin/asn1c-oai` 包装器——版本/后处理不对会重新引入 asn1c 不兼容
3. **不定义 `SIMDE_X86_*_NATIVE`**——那会让 SIMDE 去包含 `immintrin.h` 等 x86 头，根本不存在。RISC-V 走 `SIMDE_ENABLE_NATIVE_ALIASES` + 软件模拟
4. **不删 `build-riscv` 下的 `*_oai_compat.h` 和 `NR_SetupRelease.h`**（不在 git）——删了要重新生成
5. **不要 `rm -rf build-riscv`**——会丢失所有已编译 `.o` 和手工修改的产物
6. **协议桩 vs 库桩要区分**：协议桩（NGAP/F1AP 等）是 asn1c 错配，不要动；库桩（BLAS/SCTP/zlib）才要换成真实库
7. **阶段一链接通过前不要做库替换**——stub 是阶段一链接通过的必要条件

### 10.2 踩过的坑
1. **vect128 丢失**：RISC-V 移植大改 sse_intrin.h 时把 upstream 的 `vect128`/`mulByConjugate128` 整段删了，导致 nr_pbch.c 编不过。修复必须补回。
2. **ext1 不能盲删**：有的结构体有 extN 成员（能编译），有的没有。必须逐个核对生成结构体实际成员再改。
3. **NR_SetupRelease.h 的 struct tag 不带 NR_ 前缀**：`SetupRelease_2173P26`（不带 NR_） vs typedef `NR_SetupRelease_2173P26_t`（带 NR_）。第一次写错多了 NR_ 导致 struct 仍不完整。
4. **X2AP v2 脚本捏造类型**：v2 脚本用了不存在的 `X2AP_IE_Value_t`，导致 struct 定义失败、后续 typedef 全部失效；还缺 `*/` 注释终止符。
5. **改 sse_intrin.h 触发 LDPC 重编**：sse_intrin.h 被几乎所有 PHY/LDPC 文件 include，改它会触发 15-30 分钟的 LDPC 慢段重编。
6. **make all vs make nr-softmodem**：`make all` 会编 NB_IoT/lte-softmodem 等无关目标（有缺失头），改用 `make nr-softmodem` 只编目标及依赖，跳过无关模块。
7. **编译被中断 ≠ 失败**：job `exit_code=-1` 是中断信号，日志末尾无 error，直接续编即可。
8. **返回 struct 的桩要匹配 ABI**：RV64 LP64D 下 24 字节 struct 通过 sret 指针返回，16 字节通过 a0/a1 返回。桩的 struct 布局必须与真实头文件完全一致。
9. **R_RISCV_JAL 跳转范围**：±1 MiB。SIMDE 展开后大函数（>1 MiB）会溢出，用 `-Os -fno-unroll-loops` 缩小。
10. **librfsimulator.so 是 dlopen 插件**：不在 `make nr-softmodem` 目标里，要单独 `make rfsimulator`。
11. **libgfortran 是 openblas 的传递依赖**：换真实 openblas 时别漏。
12. **link.txt 被 CMake 覆盖**：手工加的 `stubs_link.o` 在 CMake 重新 configure 时会丢。
13. **协议桩的 NULL 解引用**：E1AP 的 `getCxtE1(){return 0;}` 被无判空调用导致 SIGSEGV。其他协议桩的 getter 可能有同样问题。
14. **gdb 调试用 sudo**：qemu 要 `CAP_SYS_NICE` 设 SCHED_FIFO；不要用 `pkill -f 'qemu-riscv64 -g 1234'`（会匹配当前 shell 自杀）。
15. **QEMU 6.2 能跑 rfsim**：之前误判"QEMU V 扩展仿真有缺陷"，gdb 证明崩点是真实代码 bug 不是 QEMU 限制。QEMU 6.2 跑 rfsim 功能正常，只是慢（~16% 实时）。

---

## 11. 时间线总览

| 阶段 | 完成内容 | 关键产出 |
|---|---|---|
| Phase 0 | 环境搭建 | toolchain、SIMDE、asn1c 包装器、OpenSSL 桩、libconfig |
| 第一波错误 | 10 个环境/ASN.1 兼容问题 | X2AP/NGAP 兼容头、fix_*.py 脚本 |
| 第二波错误 | SIMD 类型丢失 + ext1 扁平化 + uper 参数 + 循环 include | vect128 补回、make nr-softmodem 决策、50 处 uper 修复 |
| 第三波错误 | 61 个编译错误（6 类） | rrc_gNB_NGAP.c stub、NR_SetupRelease.h 184 #define |
| 链接阶段 | stub 库 + 9 个 undefined + JAL 溢出 | stubs.c/stubs_link.o、nr_ulsch 体积优化 |
| 52% 中断 | 账号积分耗尽，手动中断（非失败） | — |
| 阶段二 | stub 库 → 真实 RISC-V 库 | openblas/lapacke/sctp/zlib 真实库 |
| 阶段三 | QEMU 运行验证 + 3 个运行期 bug | E1AP dummy、librfsimulator.so、libgfortran.so.5 |
| **最终** | **rfsim 跑通** | **gNB 空载小区在 QEMU 持续运行** |

---

## 12. 下一步路线（优先级排序）

1. **接 UE 验证 RRC**：跑 `nr-uesoftmodem --rfsim` 连当前 gNB，看随机接入 + RRC 连接能否建立。这是验证 RRC 编解码是否真工作的唯一方法。
2. **修 RRC 专用消息编解码**：若接 UE 时崩在 RRC，按 asn1c `ANY_t`/union/ext 扁平化问题逐个修。SIB1 碰巧过了，但 RRCSetup/Reconfig 等专用消息的 UPER 编解码未验证。
3. **上 K3 板**：把 `nr-softmodem` + `librfsimulator.so` + `riscv-libs/lib/*.so` + OAI 内部 .so 一起拷板子，原生跑 rfsim 验证实时性。协议桩保持 no-op（`--noS1` 不需要核心网）。
4. **协议栈去桩**（若要核心网）：修 asn1c 接口错配，恢复 NGAP/F1AP/E1AP 真实现。
5. **strip ELF**：121 MB 偏大，上板前 `riscv64-linux-gnu-strip` 缩小。

---

*文档生成日期：2026-08-10*
*项目状态：rfsim 在 QEMU 上跑通（空载小区），距离功能完整还有协议栈去桩 + UE 接入 + 核心网 + 实时平台几大步*
