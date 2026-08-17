# 周报（第一周）：OAI 5G RISC-V 移植 —— 环境搭建与交叉编译

**汇报周期**：2026-08-03 ~ 2026-08-09
**项目目标**：将 OpenAirInterface 5G gNB（`nr-softmodem`）移植到 RISC-V 架构（进迭时空 K3 板）
**本周里程碑**：从零开始完成交叉编译环境搭建，成功生成 RISC-V `nr-softmodem` ELF 可执行文件

---

## 一、本周目标

1. 搭建 RISC-V 交叉编译环境（工具链、SIMD 模拟层、依赖库）
2. 解决 OAI 源码与 RISC-V 架构的兼容性问题
3. 完成编译并进入链接阶段

## 二、完成工作概述

本周从零开始搭建 RISC-V 移植基础，解决了 **81 个编译错误**（分三波）和链接阶段问题，最终成功生成 RISC-V `nr-softmodem` ELF（121 MB，含调试信息）。

**核心成果**：
```
nr-softmodem: ELF 64-bit LSB pie executable, UCB RISC-V, RVC, double-float ABI
Machine: RISC-V   Class: ELF64
```

---

## 三、详细进展

### 3.1 环境搭建（Phase 0）

#### 3.1.1 交叉编译工具链
- 编写 [cmake_targets/riscv64-toolchain.cmake](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-toolchain.cmake)，指定 `riscv64-linux-gnu-gcc` 交叉编译器
- 编写 [cmake_targets/riscv64-gcc-wrapper.sh](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-gcc-wrapper.sh)，过滤 CMake 误加的 x86 多架构 include 路径
- 编译 flags：`-march=rv64gcv -mabi=lp64d`

#### 3.1.2 SIMD 模拟层（核心创新）
OAI 的 PHY 层大量使用 x86 SSE/AVX intrinsic（`_mm_*`、`__m128i`），RISC-V 没有这些。解决方案：
- 引入 **SIMDE 库**，把 x86 intrinsic 调用翻译成 RISC-V V 扩展（RV64V）
- 大改 [openair1/PHY/sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h)：
  - 定义 `OAI_SIMD_X86_EMULATION` 宏
  - 做 `__m128i` → `simde__m128i` 类型映射
  - 开 `SIMDE_ENABLE_NATIVE_ALIASES`（让 `_mm_*` 解析到 SIMDE 模拟实现）
- **关键约束**：不定义 `SIMDE_X86_*_NATIVE`（那会让 SIMDE 去包含 `immintrin.h` 等 x86 头，根本不存在）

#### 3.1.3 依赖库
- 从 Ubuntu ports 下载 riscv64 的 deb 包解压到 [/home/kongbai/riscv-libs/](file:///home/kongbai/riscv-libs)，解决 `libconfig` 交叉编译找不到的问题
- 编写 [cmake_targets/riscv64-stubs/](file:///home/kongbai/openairinterface5g/cmake_targets/riscv64-stubs) OpenSSL 桩库，补全 RISC-V sysroot 缺失的头文件

### 3.2 编译阶段（三波错误，共 81 个）

#### 第一波：环境/工具链/ASN.1 兼容层（10 个问题）
主要问题及解决方法：
| 问题 | 解决方法 |
|---|---|
| `X2AP_InitiatingNodeType_EndcX2Setup_t` 未知类型 | `X2AP_oai_compat.h` 补 typedef struct |
| `asn1cSeqAdd` 宏参数数不匹配 | `oai_asn1.h` 重定义宏支持 2 参 |
| NGAP/X2AP 生成代码用 `ANY_t`，OAI 用 union | 改 ASN.1 + 改生成头 + compat 头 |
| CMake 误加 x86 多架构 include 路径 | gcc 包装器 + toolchain `-isystem` 优先级 |
| OpenSSL / `gnu/stubs.h` 等 RISC-V 缺失头 | riscv64-stubs 桩库 |

**辅助脚本**（放在 [riscv-port-scripts/](file:///home/kongbai/openairinterface5g/riscv-port-scripts)）：
- `fix_x2ap_compat_v2.py` —— 扫描 X2AP 源码生成兼容头
- `fix_all_constants.py` / `fix_x2ap_constants.py` —— 补全枚举常量
- `fix_ngap_*.py` —— NGAP 编码器/兼容头修复

#### 第二波：SIMD 类型丢失 + 源码 ext1 扁平化 + uper 参数（约 10 个问题）
- **`vect128` / `mulByConjugate128` 未定义**：RISC-V 移植大改 `sse_intrin.h` 时整段删掉了，在文件末尾补回
- **`ext1` 扩展成员访问**：asn1c 生成的结构体把 ASN.1 扩展成员拍平成直接成员，去掉 `->ext1->` 间接层
- **`uper_encode_to_buffer` 参数数不匹配**：OAI 源码用 5 参调用，asn1c 生成 4 参，用 Python 脚本统一修（50 处替换跨 15 个文件）
- **关键决策**：改用 `make nr-softmodem` 替代 `make all`，只构建目标及依赖，跳过 NB_IoT/lte-softmodem 等无关模块

#### 第三波：61 个编译错误（集中爆发在 NGAP/NR RRC 区域）
| 类别 | 错误数 | 解决方法 |
|---|---|---|
| NGAP 类型/常量缺失 | 30 | `rrc_gNB_NGAP.c` 整文件打成 stub |
| NR SetupRelease 不完整类型 | 18 | `NR_SetupRelease.h` 末尾插入 184 个 `#define` |
| `ext1`/`ext4` 拍平扩展 | 7 | 去掉间接层 |
| M3AP stub .h 清理 | 6 | 函数参数改为 `void *` |
| gnb_config 混杂 | 9 | SetupRelease + struct + NR_ 前缀 |

**最 tricky 的修复**：NR_SetupRelease 是 ASN.1 参数化类型 `SetupRelease<T>`，asn1c 生成 `SetupRelease_2173PXX`，但 OAI 源码用 `NR_SetupRelease_PUSCH_Config` 等名字。关键细节是 struct tag 不带 NR_ 前缀，typedef 带 NR_ 前缀。

### 3.3 链接阶段

#### 缺失库（stub 共享库）
RISC-V 交叉编译环境缺少 4 个库，用空实现 stub .so 替代：
- `libsctp.so` —— SCTP 协议（空实现返回 -1/0）
- `libblas.so` / `liblapacke.so` —— 线性代数运算
- `libz.so` —— zlib 压缩

#### 9 个未定义符号
```
der_write_tags / ber_check_tags / der_encode / ber_decode / asn_imax2INTEGER
OPENSSL_assert
__builtin_cpu_init / __builtin_cpu_supports
s1ap_config
```
**解决**：在 [stubs.c](file:///home/kongbai/riscv-libs/stubs.c) 增加桩实现，单独抽出 [stubs_link.c](file:///home/kongbai/riscv-libs/stubs_link.c) → `stubs_link.o` 静态链入。

**关键细节**：返回 struct 的桩必须匹配 RV64 LP64D ABI（24 字节 struct 通过 sret 指针返回，16 字节通过 a0/a1 返回）。

#### R_RISCV_JAL 重定位溢出
```
relocation truncated to fit: R_RISCV_JAL against `nr_ulsch_qam64_qam64'
```
**根因**：`nr_ulsch_llr_computation.c` 里的 QAM 解调函数经 SIMDE 展开后膨胀到 **1.63 MiB**，超过 RISC-V `JAL` 指令 ±1 MiB 跳转范围。
**解决**：对该文件单独用 `-Os -fno-unroll-loops` 重编，函数体积从 1.63 MiB 降到 704 KiB。

---

## 四、本周成果

| 成果 | 状态 |
|---|---|
| RISC-V 交叉编译环境搭建 | ? 完成 |
| SIMD 模拟层（SIMDE）集成 | ? 完成 |
| 81 个编译错误全部解决 | ? 完成 |
| 链接阶段问题全部解决 | ? 完成 |
| RISC-V `nr-softmodem` ELF 生成 | ? 完成（121 MB） |

## 五、下周计划

1. 用真实 RISC-V 库替换 stub 库（BLAS/LAPACK/SCTP/zlib）
2. 在 QEMU 上运行验证 `nr-softmodem --rfsim --noS1`
3. 修复运行期 bug，跑通 gNB rfsim 空载小区

## 六、风险与问题

- **协议桩未去桩**：NGAP/S1AP/F1AP/E1AP 等核心网协议暂用 no-op stub，只能跑 `--noS1` 模式
- **link.txt 被 CMake 覆盖**：手工加的 `stubs_link.o` 在 CMake 重新 configure 时会丢，需重加
- **QEMU 运行尚未验证**：ELF 生成成功但未在 QEMU 上实际运行，可能有运行期问题
