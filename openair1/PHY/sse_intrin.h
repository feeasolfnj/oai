#ifndef __SSE_INTRIN_H__
#define __SSE_INTRIN_H__

/*
 * sse_intrin.h - Cross-platform SIMD intrinsic header for OAI
 *
 * On x86: provides native SSE/AVX intrinsics via immintrin.h
 * On RISC-V: provides SIMD emulation via SIMDE (maps x86 intrinsics to RVV)
 *            When OAI_SIMD_X86_EMULATION is defined, the x86 code path
 *            is selected by the source files, and this header provides
 *            the SIMDE equivalents of x86 intrinsics.
 * On ARM: provides NEON intrinsics
 */

#if defined(OAI_SIMD_X86_EMULATION)
  /*
   * RISC-V (or other non-x86) with x86 SIMD emulation.
   * The source code uses #if defined(__x86_64__) || defined(__i386__)
   * to select x86 SIMD code paths. We define these macros so the
   * x86 code is compiled, and provide SIMDE replacements.
   */
  #define __x86_64__
  #undef __i386__

  /* Tell SIMDE we do NOT have native x86 SSE/AVX support;
   * we want SIMDE's software emulation (using RVV on RISC-V).
   * Without this, SIMDE sees __x86_64__ and assumes native SSE2,
   * which causes _mm_* intrinsics to reference x86-only builtins. */
  #define SIMDE_NO_NATIVE
  #define SIMDE_X86_SSE_NO_NATIVE
  #define SIMDE_X86_SSE2_NO_NATIVE
  #define SIMDE_X86_SSE3_NO_NATIVE
  #define SIMDE_X86_SSSE3_NO_NATIVE
  #define SIMDE_X86_SSE4_1_NO_NATIVE
  #define SIMDE_X86_SSE4_2_NO_NATIVE
  #define SIMDE_X86_AVX_NO_NATIVE
  #define SIMDE_X86_AVX2_NO_NATIVE
  #define SIMDE_X86_FMA_NO_NATIVE
  #define SIMDE_X86_AVX512F_NO_NATIVE

  /* Enable SIMDE native aliases so _mm_* maps to simde_mm_* */
  #ifndef SIMDE_ENABLE_NATIVE_ALIASES
    #define SIMDE_ENABLE_NATIVE_ALIASES
  #endif
  #ifndef SIMDE_ENABLE_OPENMP
    #define SIMDE_ENABLE_OPENMP
  #endif

  /* Include SIMDE headers that provide x86 intrinsic emulation */
  #if __has_include(<simde/x86/sse2.h>)
    #include <simde/x86/sse2.h>
  #endif
  #if __has_include(<simde/x86/avx2.h>)
    #include <simde/x86/avx2.h>
  #endif
  #if __has_include(<simde/x86/ssse3.h>)
    #include <simde/x86/ssse3.h>
  #endif
  #if __has_include(<simde/x86/sse4_2.h>)
    #include <simde/x86/sse4_2.h>
  #endif

  /* Map x86 type names to SIMDE equivalents */
  typedef simde__m128i  __m128i;
  typedef simde__m128   __m128;
  typedef simde__m256i  __m256i;
  typedef simde__m256   __m256;
  typedef simde__m64    __m64;

  /* ARM NEON intrinsic emulation for mixed code (vqsubq_s16 etc.) */
  #if __has_include(<simde/arm/neon/qadd.h>)
    #include <simde/arm/neon/qadd.h>
  #endif
  #if __has_include(<simde/arm/neon/qsub.h>)
    #include <simde/arm/neon/qsub.h>
  #endif
  #if __has_include(<simde/arm/neon/qshl.h>)
    #include <simde/arm/neon/qshl.h>
  #endif

  /* Map ARM NEON types to SIMDE equivalents for mixed code */
  typedef simde_int16x8_t  int16x8_t;
  typedef simde_int32x4_t  int32x4_t;
  typedef simde_uint32x4_t uint32x4_t;
  typedef simde_float32x4_t float32x4_t;

  /* Map NEON intrinsics used in mixed x86/NEON code */
  #define vqsubq_s16(a, b)  simde_vqsubq_s16(a, b)
  #define vqaddq_s16(a, b)  simde_vqaddq_s16(a, b)
  #define vqshlq_s16(a, b)  simde_vqshlq_s16(a, b)
  #define vqcgtq_s16(a, b)  simde_vqcgtq_s16(a, b)

#elif defined(__x86_64__) || defined(__i386__)
  /* Native x86 SIMD intrinsics */
  #include <immintrin.h>
  #include <x86intrin.h>

#elif defined(__arm__) || defined(__aarch64__)
  /* ARM NEON intrinsics */
  #include <arm_neon.h>

#elif defined(__riscv) && (__riscv_xlen == 64)
  /* RISC-V without x86 emulation - provide basic SIMDE types */
  #if __has_include(<simde/x86/sse2.h>)
    #include <simde/x86/sse2.h>
  #endif
  #if __has_include(<simde/x86/avx2.h>)
    #include <simde/x86/avx2.h>
  #endif

  typedef simde__m128i  __m128i;
  typedef simde__m128   __m128;
  typedef simde__m256i  __m256i;
  typedef simde__m256   __m256;
  typedef simde__m64    __m64;

  #ifndef SIMDE_ENABLE_NATIVE_ALIASES
    #define SIMDE_ENABLE_NATIVE_ALIASES
  #endif

#else
  /* Fallback: scalar-only compilation */
  typedef int32_t  __m128i __attribute__((vector_size(16)));
  typedef float    __m128  __attribute__((vector_size(16)));
  typedef int32_t  __m256i __attribute__((vector_size(32)));
  typedef float    __m256  __attribute__((vector_size(32)));
  typedef int64_t  __m64;
#endif

/*
 * OAI specific SIMD helpers
 * (vect128 / mulByConjugate128 were present in the upstream header and are
 *  used by nr_pbch.c. Under OAI_SIMD_X86_EMULATION, __x86_64__ is defined
 *  above so the x86 branch is selected; _mm_* intrinsics are provided by
 *  SIMDE via SIMDE_ENABLE_NATIVE_ALIASES, and __m128i is already typedef'd
 *  to simde__m128i. The __riscv branch is kept for the non-emulation path.)
 */
#if defined(__x86_64__) || defined(__i386__)
  #define vect128 __m128i
#elif defined(__arm__) || defined(__aarch64__)
  #define vect128 int16x8_t
#elif defined(__riscv) && (__riscv_xlen == 64)
  #define vect128 simde__m128i
#endif

static const short minusConjug128[8] __attribute__((aligned(16))) = {-1,1,-1,1,-1,1,-1,1};
static inline vect128 mulByConjugate128(vect128 *a, vect128 *b, int8_t output_shift) {
#if defined(__x86_64__) || defined(__i386__)
  vect128 realPart = _mm_madd_epi16(*a,*b);
  realPart = _mm_srai_epi32(realPart,output_shift);
  vect128 imagPart = _mm_shufflelo_epi16(*b,_MM_SHUFFLE(2,3,0,1));
  imagPart = _mm_shufflehi_epi16(imagPart,_MM_SHUFFLE(2,3,0,1));
  imagPart = _mm_sign_epi16(imagPart,*(vect128 *)minusConjug128);
  imagPart = _mm_madd_epi16(imagPart,*a);
  imagPart = _mm_srai_epi32(imagPart,output_shift);
  vect128 lowPart = _mm_unpacklo_epi32(realPart,imagPart);
  vect128 highPart = _mm_unpackhi_epi32(realPart,imagPart);
  return ( _mm_packs_epi32(lowPart,highPart));
#elif defined(__arm__) || defined(__aarch64__)
  AssertFatal(false, "not developped\n");
#elif defined(__riscv) && (__riscv_xlen == 64)
  vect128 realPart = simde_mm_madd_epi16(*a,*b);
  realPart = simde_mm_srai_epi32(realPart,output_shift);
  vect128 imagPart = simde_mm_shufflelo_epi16(*b,0x1B);
  imagPart = simde_mm_shufflehi_epi16(imagPart,0x1B);
  imagPart = simde_mm_sign_epi16(imagPart,*(vect128 *)minusConjug128);
  imagPart = simde_mm_madd_epi16(imagPart,*a);
  imagPart = simde_mm_srai_epi32(imagPart,output_shift);
  vect128 lowPart = simde_mm_unpacklo_epi32(realPart,imagPart);
  vect128 highPart = simde_mm_unpackhi_epi32(realPart,imagPart);
  return (simde_mm_packs_epi32(lowPart,highPart));
#endif
}

#endif /* __SSE_INTRIN_H__ */
