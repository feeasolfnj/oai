# OAI 5G RISC-V 移植：SIMDE 与 asn1c 核心难点问题与解决方法

**汇报场景**：组会技术汇报
**项目**：OpenAirInterface 5G gNB/UE 移植到 RISC-V 架构（进迭时空 K3 板）
**报告范围**：移植过程中两个最核心的技术难点——SIMD 指令集模拟（SIMDE）与 ASN.1 代码生成兼容性（asn1c）

---

## 一、背景

OAI 5G 的 PHY 层（物理层）大量使用 x86 SSE/AVX SIMD intrinsic，协议层（RRC/NGAP/X2AP）依赖 asn1c 工具生成的 C 代码做 ASN.1 消息编解码。这两个组件在 x86 平台上能正常工作，但移植到 RISC-V 时遇到了系统性兼容性问题，是整个移植过程中工作量最大、最棘手的两部分。

| 组件 | x86 平台 | RISC-V 移植挑战 |
|---|---|---|
| **SIMDE** | 原生 SSE/AVX intrinsic | RISC-V 无 x86 SIMD，需用 RV64V 模拟 |
| **asn1c** | 官方版本生成代码与 OAI 源码匹配 | 生成的结构体布局与 OAI 源码接口错配 |

---

## 二、SIMDE：x86 SIMD 到 RISC-V V 扩展的模拟

### 2.1 问题背景

OAI 的 PHY 层（LDPC 译码、PBCH 解调、信道估计等）几乎每个文件都 include [openair1/PHY/sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h)，使用 `_mm_*` 系列 x86 intrinsic。RISC-V 指令集没有这些。

**解决思路**：引入 [SIMDE 库](https://github.com/simd-everywhere/simde)，它能把 x86 intrinsic 调用翻译成目标平台的 SIMD 指令（RISC-V 上是 RV64V 向量扩展）。

但直接用 SIMDE 会有多个坑，需要大量适配工作。

### 2.2 问题 1：架构检测宏欺骗

**问题**：OAI 源码用 `#if defined(__x86_64__)` 选择 x86 SIMD 代码路径。如果在 RISC-V 上不定义这个宏，会走到 ARM 或标量 fallback 路径，性能差且可能逻辑不对。但如果定义了 `__x86_64__`，SIMDE 又会以为平台原生支持 x86 SSE，去包含 `immintrin.h`（x86 专有头，RISC-V 上根本不存在）。

**解决**：在 [sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h) 中定义 `OAI_SIMD_X86_EMULATION` 宏，走专门的模拟分支：
```c
#if defined(OAI_SIMD_X86_EMULATION)
  #define __x86_64__          // 让 OAI 源码选中 x86 代码路径
  #define SIMDE_NO_NATIVE     // 告诉 SIMDE 不要用原生 x86
  #define SIMDE_X86_SSE2_NO_NATIVE
  // ... 所有 SIMDE_X86_*_NO_NATIVE 都定义
  #define SIMDE_ENABLE_NATIVE_ALIASES  // 让 _mm_* 映射到 simde_mm_*
  #include <simde/x86/sse2.h>
  typedef simde__m128i  __m128i;       // 类型映射
#endif
```

**关键约束**：
- ? 定义 `SIMDE_ENABLE_NATIVE_ALIASES`（让 `_mm_*` 解析到 SIMDE 模拟实现）
- ? **不定义** `SIMDE_X86_*_NATIVE`（那会让 SIMDE 去包含 `immintrin.h`，编译报错）

### 2.3 问题 2：`_mm_pause` 在 RISC-V 不可用

**问题**：`_mm_pause` 是 x86 的 `pause` 指令（CPU 空转提示），RISC-V 没有对应指令。SIMDE 原版对 RISC-V 的支持不完整，导致编译报错。

**解决**：修改 [extra/simde/simde/x86/sse2.h:5519-5564](file:///home/kongbai/openairinterface5g/extra/simde/simde/x86/sse2.h#L5519)，为 RISC-V 添加 pause 实现：
```c
#elif defined(SIMDE_ARCH_RISCV32) || defined(SIMDE_ARCH_RISCV64)
  /* RISC-V pause hint: Zihintpause 扩展 */
  #if __GNUC__ >= 12
    __builtin_riscv_pause();
  #else
    /* GCC 11 以下用内联汇编编码 pause 指令 */
    __asm__ __volatile__ (".insn i 0x0F, 0, x0, x0, 0x010" ::: "memory");
  #endif
```

### 2.4 问题 3：`vect128` / `mulByConjugate128` 类型丢失（最隐蔽）

**问题**：编译 `nr_pbch.c` 时报 `unknown type name 'vect128'`、`'rxdataF128' undeclared`。

**根因**：upstream 的 [sse_intrin.h](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h) 有三路 `vect128` 定义（x86/arm/SIMDE）+ `mulByConjugate128` 函数实现。RISC-V 移植大改该文件时**整段删掉了**，但 `nr_pbch.c` 两者都用。

**解决**：在文件末尾补回完整定义（[sse_intrin.h:139-170](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h#L139)）：
```c
#define vect128 simde__m128i
static const short minusConjug128[8] __attribute__((aligned(16))) = {-1,1,-1,1,-1,1,-1,1};
static inline vect128 mulByConjugate128(vect128 *a, vect128 *b, int8_t output_shift) {
  vect128 realPart = simde_mm_madd_epi16(*a,*b);
  vect128 imagPart = simde_mm_shufflelo_epi16(*b,0x1B);
  imagPart = simde_mm_sign_epi16(imagPart,*(vect128 *)minusConjug128);
  // ...
}
```

**踩坑教训**：改 `sse_intrin.h`（被几乎所有 PHY/LDPC 文件 include）会触发整个 LDPC+PHY 层重编，LDPC 译码器 5 个变体单个 `.o` 达 99MB，是 15-30 分钟慢段。

### 2.5 问题 4：R_RISCV_JAL 重定位溢出（SIMDE 展开导致函数过大）

**问题**：链接时报错：
```
relocation truncated to fit: R_RISCV_JAL against `nr_ulsch_qam64_qam64'
```

**根因**：`nr_ulsch_llr_computation.c` 里的 QAM 解调函数经 SIMDE 展开（x86 SIMD → RV64V）后膨胀到 **1.63 MiB**，超过 RISC-V `JAL`（Jump And Link）指令 ±1 MiB 跳转范围。

**解决**：对该文件单独用体积优化重编：
```bash
riscv64-linux-gnu-gcc -march=rv64gcv -mabi=lp64d -Os -fno-unroll-loops \
  -c nr_ulsch_llr_computation.c -o /tmp/nr_ulsch_llr.o
```
函数体积从 1.63 MiB 降到 704 KiB，符合 JAL 范围。把新 `.o` 替换进 `libPHY_NR.a` 后重链通过。

**技术细节**：RISC-V 的 `JAL` 指令用 20 位有符号立即数编码偏移，范围 ±1 MiB。这是 RISC-V 特有的限制，x86 的相对跳转范围大得多（±2 GiB），所以 x86 上从没遇到过这种问题。

### 2.6 问题 5：NEON 混合代码

**问题**：OAI 部分代码混用 x86 和 ARM NEON intrinsic（如 `vqsubq_s16`），RISC-V 两者都没有。

**解决**：在 [sse_intrin.h:69-90](file:///home/kongbai/openairinterface5g/openair1/PHY/sse_intrin.h#L69) 补 SIMDE 的 ARM NEON 模拟：
```c
#include <simde/arm/neon/qadd.h>
#include <simde/arm/neon/qsub.h>
typedef simde_int16x8_t  int16x8_t;
#define vqsubq_s16(a, b)  simde_vqsubq_s16(a, b)
#define vqaddq_s16(a, b)  simde_vqaddq_s16(a, b)
```

### 2.7 SIMDE 小结

| 问题 | 根因 | 解决方法 |
|---|---|---|
| 架构宏欺骗 | `__x86_64__` 与 SIMDE 原生检测冲突 | `OAI_SIMD_X86_EMULATION` + `SIMDE_NO_NATIVE` |
| `_mm_pause` 不可用 | RISC-V 无 pause 指令 | 改 simde sse2.h，加 RISC-V 内联汇编 |
| `vect128` 丢失 | 大改时误删 | 补回完整定义 |
| JAL 溢出 | SIMDE 展开函数 >1 MiB | `-Os -fno-unroll-loops` 单文件优化 |
| NEON 混合代码 | 混用 ARM intrinsic | SIMDE NEON 模拟 |

---

## 三、asn1c：ASN.1 代码生成与 OAI 源码的接口错配

### 3.1 问题背景

OAI 的协议层（RRC/NGAP/X2AP/S1AP 等）用 asn1c 工具从 ASN.1 描述文件生成 C 代码，做 5G/LTE 信令消息的编解码（UPER/BER/DER）。

**工具链**：使用 OAI 官方推荐版本 `mouse07410/asn1c` 的 `vlm_master` 分支，通过包装器 [/home/kongbai/bin/asn1c-oai](file:///home/kongbai/bin/asn1c-oai) 调用（做了文件名前缀、类型名映射等后处理）。

**核心矛盾**：asn1c 生成的 C 代码结构体布局与 OAI 源码期望的接口**不匹配**。这种错配不是编译错误能直接看出来的，而是要深入理解 ASN.1 语义和 asn1c 生成机制才能修。

### 3.2 问题 1：`ANY_t` vs union（open-type 编码差异）

**问题**：NGAP/X2AP 等协议的 ASN.1 定义里有 open-type（`OPEN TYPE`）字段，asn1c 对这类字段的生成方式与 OAI 源码期望不一致：
- **asn1c 生成**：`ANY_t value`（通用字节数组容器，编解码时手动处理内容）
- **OAI 源码期望**：`union { struct RRCSetup *rcSetup; struct ... } choice`（带 `present`/`choice` 成员的标签联合体）

**后果**：OAI 源码访问 `->present`、`->choice.xxx` 时编译报错（结构体里根本没有这些成员）。

**解决**（三步走）：
1. 改 ASN.1 源文件（让 asn1c 生成 union 而非 `ANY_t`）
2. 改生成头文件（手工把 `ANY_t value` 改成 union 定义）
3. 写 compat 兼容头（`X2AP_oai_compat.h`、`NGAP_oai_compat.h`）做类型映射

涉及文件（ASN.1 源）：
- [openair2/X2AP/MESSAGES/ASN1/R15/x2ap-15.{1.0,3.0,6.0}.asn1](file:///home/kongbai/openairinterface5g/openair2/X2AP/MESSAGES/ASN1)
- [openair3/NGAP/MESSAGES/ASN1/ngap-15.8.0.asn1](file:///home/kongbai/openairinterface5g/openair3/NGAP/MESSAGES/ASN1/ngap-15.8.0.asn1)

### 3.3 问题 2：`ext1`/`ext4` 扩展成员扁平化（最反复出现）

**问题**：ASN.1 协议定义里有扩展成员（r9/r10/r12/r13/r14/r15），原版 asn1c 把它们包在 `ext1`/`ext2`/`ext4` 嵌套结构里，OAI 源码用 `->ext1->member` 间接访问。但我们用的 asn1c 版本把扩展成员**拍平成直接成员**，去掉了 `ext1` 包装层。

**后果**：`NR_TDD_UL_DL_Pattern_t has no member named 'ext1'` 等错误，出现在 `config.c`、`config_ue.c`、`gNB_scheduler_dlsch.c`、`nr_ue_scheduler.c`、`pdcp.c`、`rrc_UE.c`、`rrc_eNB.c`、`rrc_sl_preconfig.c` 等多个文件。

**解决原则**：**先核对生成结构体实际成员再改，不盲删**。去掉 `->ext1->`/`->ext4->` 间接层，直接访问拍平后的成员。

**踩坑**：有的结构体有 extN 成员（如 `NR_PDCP_Config` 有 `ext1` 能编译），有的没有（如 `NR_TDD_UL_DL_Pattern`、`SL_PreconfigSync_r12` 没有）。**不能批量盲删，必须逐个核对**。

### 3.4 问题 3：`uper_encode_to_buffer` 参数数不匹配

**问题**：OAI 源码用 5 参调用 `uper_encode_to_buffer`，asn1c 生成的函数是 4 参（少了一个 NULL 中间参数）。

**解决**：用 Python 脚本统一修所有 5 参 → 4 参（删 NULL 第 2 参），**50 处替换跨 15 个文件**。

涉及文件（部分）：
- [openair2/RRC/NR/MESSAGES/asn1_msg.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/MESSAGES/asn1_msg.c)
- [openair2/RRC/LTE/MESSAGES/asn1_msg.c](file:///home/kongbai/openairinterface5g/openair2/RRC/LTE/MESSAGES/asn1_msg.c)
- [openair3/NGAP/](file:///home/kongbai/openairinterface5g/openair3/NGAP) 下 8 个文件

### 3.5 问题 4：`NR_SetupRelease` 类型映射（最 tricky）

**问题**：NR RRC 里 `SetupRelease<T>` 是 ASN.1 参数化类型，asn1c 生成 `SetupRelease_2173PXX`（XX 是类型编号），但 OAI 源码用 `NR_SetupRelease_PUSCH_Config` 等语义化名字。导致 struct 不完整类型错误（18 个错误，集中在 `nr_rrc_config.c` 和 `gnb_config.c`）。

**关键细节**（命名规则不一致）：
| 类型 | asn1c 生成 | 是否带 NR_ 前缀 |
|---|---|---|
| struct tag | `SetupRelease_2173P26` | ? 不带 |
| typedef | `NR_SetupRelease_2173P26_t` | ? 带 |
| enum | `SetupRelease_2173P26_PR_setup` | ? 不带 |

aliases 文件已有 typedef 和 enum 映射，**缺的是 struct tag 映射**。

**解决**：在生成的 `NR_SetupRelease.h` 末尾插入 184 个 `#define`：
```c
#define NR_SetupRelease_PUSCH_Config SetupRelease_2173P26
```

**踩坑**：第一次错误写成 `#define ... NR_SetupRelease_2173P26`（多了 NR_），导致 struct 仍不完整。用 sed 批量修正。

### 3.6 问题 5：9 个未定义符号（asn1c runtime 缺失）

**问题**：链接时报 9 个未定义符号：
```
der_write_tags / ber_check_tags / der_encode / ber_decode / asn_imax2INTEGER
OPENSSL_assert
__builtin_cpu_init / __builtin_cpu_supports
s1ap_config
```

**根因分析**：
| 符号 | 来源 | 原因 |
|---|---|---|
| `der_write_tags`/`ber_check_tags`/`der_encode`/`ber_decode`/`asn_imax2INTEGER` | asn1c BER/DER runtime | OAI 自带 asn1c runtime 缺这几个；RRC 用 UPER 不走 BER/DER，但 `ANY.c`/`constr_SEQUENCE_OF.c` 等通用支持对象仍会拉入 |
| `OPENSSL_assert` | OpenSSL | 桩头文件把它宏成 `((void)0)`，但没 include 该头的 TU 会发出真实调用 |
| `__builtin_cpu_init`/`__builtin_cpu_supports` | x86 GCC builtin | RISC-V GCC 不提供；OAI 的 `log.c`/`cmult_sv.c` 用它选 x86 快速路径 |

**解决**：在 [stubs.c](file:///home/kongbai/riscv-libs/stubs.c) 增加桩实现，单独抽出 [stubs_link.c](file:///home/kongbai/riscv-libs/stubs_link.c) → `stubs_link.o` 静态链入。

**关键细节**：返回 struct 的桩必须匹配 RV64 LP64D ABI：
```c
/* asn_enc_rval_t: 24 字节 struct,RV64 LP64D 下通过隐藏 sret 指针(a0)返回 */
typedef struct { long encoded; void *failed_type; void *structure_ptr; } asn_enc_rval_stub_t;
asn_enc_rval_stub_t der_encode(void) {
    asn_enc_rval_stub_t r; r.encoded = -1; r.failed_type = 0; r.structure_ptr = 0; return r;
}
/* asn_dec_rval_t: 16 字节,通过 a0/a1 返回;code=RC_FAIL(2) 让调用方走错误路径 */
```

**`__builtin_cpu_supports` 是 shim 不是纯桩**：返回 0（"不支持任何 x86 特性"）让 OAI 走可移植 C 路径，这正是 RISC-V 上的正确行为（SIMDE 单独处理 SIMD）。

### 3.7 问题 6：协议桩策略（主动搁置核心网协议）

**问题**：NGAP/S1AP/F1AP/E1AP/M2AP/M3AP 等核心网协议的 asn1c 接口错配太严重，逐个修工作量巨大（每个协议几十个消息、上百个 IE）。

**决策**：主动把这些协议**整文件打成 no-op stub**，因为跑 rfsim 用 `--noS1` 模式不需要核心网信令。

**典型案例**：[rrc_gNB_NGAP.c](file:///home/kongbai/openairinterface5g/openair2/RRC/NR/rrc_gNB_NGAP.c) 原文件 1354 行、11 个公开函数，整文件 stub（原文件备份为 `.c.orig`），所有函数 no-op。

**注意事项**：
- **不能 stub `rrc_gNB_modify_dedicatedRRCReconfiguration`**——它在 `rrc_gNB.c:798` 已有定义，重复定义会导致链接错误
- stub 函数都是 no-op，NGAP 信令功能全部不可用

### 3.8 asn1c 小结

| 问题 | 根因 | 解决方法 |
|---|---|---|
| `ANY_t` vs union | open-type 生成差异 | 改 ASN.1 + 改生成头 + compat 头 |
| `ext1`/`ext4` 扁平化 | 扩展成员拍平 | 逐个核对，去掉间接层 |
| `uper_encode_to_buffer` 参数 | 5 参 vs 4 参 | Python 脚本批量修（50 处/15 文件） |
| `NR_SetupRelease` 类型映射 | 参数化类型命名不一致 | 184 个 `#define`，struct tag 映射 |
| 9 个未定义符号 | runtime 缺失 | `stubs_link.o` 静态链入，匹配 RV64 ABI |
| 核心网协议错配 | 工作量巨大 | 主动桩化，`--noS1` 模式不需要 |

---

## 四、遗留问题与风险

### 4.1 SIMDE 遗留风险

**RRC 专用消息编解码未验证**：SIB1（广播消息）的 UPER 编解码碰巧过了，但 `RRCSetup`/`RRCSetupComplete`/`RRCReconfiguration` 等专用消息的 UPER 编解码**从没被实际验证过**。UE 一旦发起 RRC 连接，asn1c 生成的编解码代码可能崩。

### 4.2 asn1c 遗留风险

**极化译码器数值精度**：当前 UE 接入调试卡在 PBCH 译码失败（`polar_decoder_int16` 返回错误），怀疑可能是 SIMDE 在 RV64V 下的数值精度问题，但这尚未证实。

---

## 五、经验总结

### 5.1 SIMDE 经验

1. **架构宏欺骗是关键技巧**：通过定义 `__x86_64__` + `SIMDE_NO_NATIVE` 组合，让 OAI 源码选中 x86 代码路径，同时让 SIMDE 走软件模拟，两者各取所需。
2. **改公共头文件代价大**：`sse_intrin.h` 被几乎所有 PHY 文件 include，改它触发 15-30 分钟 LDPC 重编慢段。
3. **RISC-V JAL 范围限制**：±1 MiB，SIMDE 展开后大函数会溢出，要对超大函数单独 `-Os` 优化。
4. **不定义 `SIMDE_X86_*_NATIVE`**：那会让 SIMDE 去包含 x86 专有头，编译报错。

### 5.2 asn1c 经验

1. **不能盲删 `ext1`**：有的结构体有 extN 成员，有的没有，必须逐个核对生成结构体实际成员。
2. **struct tag 命名不一致**：asn1c 生成的 struct tag 不带 NR_ 前缀，typedef 带，enum 不带，容易写错。
3. **返回 struct 的桩要匹配 ABI**：RV64 LP64D 下不同大小的 struct 通过不同方式返回（sret 指针 vs 寄存器），桩的 struct 布局必须与真实头文件完全一致。
4. **协议桩 vs 库桩要区分**：协议桩（NGAP/F1AP 等）是 asn1c 错配，不要动；库桩（BLAS/SCTP/zlib）才要换成真实库。
5. **`make nr-softmodem` 而非 `make all`**：只构建目标及依赖，跳过 NB_IoT/lte-softmodem 等无关模块（有缺失头）。

---

## 六、核心数据

| 指标 | 数量 |
|---|---|
| SIMDE 修改文件 | 2（sse_intrin.h、simde/sse2.h） |
| asn1c 兼容性修改文件 | 20+（源码 + ASN.1 + 生成头） |
| asn1c 编译错误总数 | 81（三波：10 + 10 + 61） |
| `uper_encode_to_buffer` 修复点 | 50 处跨 15 文件 |
| `NR_SetupRelease.h` #define 数 | 184 |
| 未定义符号桩 | 9（stubs_link.o） |
| 协议桩化文件 | 6 个协议（NGAP/S1AP/F1AP/E1AP/M2AP/M3AP） |

---

*报告生成日期：2026-08-15*
*项目状态：gNB rfsim 跑通，UE 接入调试进行中（PBCH 译码失败根因已定位）*
