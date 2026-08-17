#ifndef CBLAS_H_RISCV_STUB
#define CBLAS_H_RISCV_STUB

/*
 * Minimal CBLAS header stub for RISC-V cross-compilation.
 *
 * The x86 host only ships cblas.h under /usr/include/x86_64-linux-gnu, which
 * the RISC-V cross-compiler cannot use (and which would pull in x86 BLAS
 * libs).  OAI's only consumer of CBLAS is
 *   openair1/PHY/LTE_UE_TRANSPORT/linear_preprocessing_rec.c
 * which uses cblas_cgemm plus the CBLAS_ORDER / CBLAS_TRANSPOSE enums.  This
 * header declares exactly that surface; the implementation lives in
 * cblas_stubs.c (naive O(M*N*K) matrix multiply -- correctness over speed,
 * per the "just run" porting goal).
 *
 * Enum values match the reference CBLAS so that any value-comparing code
 * stays consistent.
 */

enum CBLAS_ORDER    { CblasRowMajor = 101, CblasColMajor = 102 };
enum CBLAS_TRANSPOSE { CblasNoTrans = 111, CblasTrans = 112, CblasConjTrans = 113 };
enum CBLAS_UPLO     { CblasUpper = 121, CblasLower = 122 };
enum CBLAS_DIAG     { CblasNonUnit = 131, CblasUnit = 132 };

void cblas_cgemm(const enum CBLAS_ORDER Order,
                 const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB,
                 const int M, const int N, const int K,
                 const void *alpha, const void *A, const int lda,
                 const void *B, const int ldb, const void *beta,
                 void *C, const int ldc);

#endif /* CBLAS_H_RISCV_STUB */
