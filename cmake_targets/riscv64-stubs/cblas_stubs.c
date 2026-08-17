/*
 * Minimal CBLAS implementation stub for RISC-V cross-compilation.
 *
 * Only cblas_cgemm (complex single-precision general matrix multiply) is
 * implemented, with a naive O(M*N*K) algorithm.  This is correctness-first,
 * not performance -- the RISC-V porting goal is "just run", and the only OAI
 * caller is openair1/PHY/LTE_UE_TRANSPORT/linear_preprocessing_rec.c (an LTE
 * UE transport helper).
 *
 * Complex numbers are passed as void* to interleaved float pairs
 * {re, im}, matching the reference CBLAS ABI.
 */
#include "cblas.h"

/* Pointer to complex element [i][j] of an MxN matrix stored with leading
 * dimension ld, under the given order. */
static inline const float *cblas_elem(const enum CBLAS_ORDER Order,
                                      const void *base, int ld, int i, int j) {
  const float *p = (const float *)base;
  return (Order == CblasRowMajor) ? p + 2 * (i * ld + j)
                                  : p + 2 * (j * ld + i);
}

static inline void cmul(const float *a, const float *b, float *r) {
  /* (ar + i*ai) * (br + i*bi) */
  float re = a[0] * b[0] - a[1] * b[1];
  r[1] = a[0] * b[1] + a[1] * b[0];
  r[0] = re;
}

void cblas_cgemm(const enum CBLAS_ORDER Order,
                 const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB,
                 const int M, const int N, const int K,
                 const void *alpha, const void *A, const int lda,
                 const void *B, const int ldb, const void *beta,
                 void *C, const int ldc) {
  const float *al = (const float *)alpha;
  const float *be = (const float *)beta;
  const float zero[2] = {0.0f, 0.0f};

  /* If alpha == 0: C = beta * C (still must scale C). */
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      float *c = (float *)cblas_elem(Order, C, ldc, i, j);

      /* acc = alpha * sum_k op(A)[i][k] * op(B)[k][j] */
      float acc[2] = {0.0f, 0.0f};
      if (!(al[0] == 0.0f && al[1] == 0.0f)) {
        for (int k = 0; k < K; k++) {
          const float *pa;
          float a_conj[2];
          if (TransA == CblasNoTrans)
            pa = cblas_elem(Order, A, lda, i, k);
          else {
            pa = cblas_elem(Order, A, lda, k, i);
            if (TransA == CblasConjTrans) {
              a_conj[0] = pa[0];
              a_conj[1] = -pa[1];
              pa = a_conj;
            }
          }

          const float *pb;
          float b_conj[2];
          if (TransB == CblasNoTrans)
            pb = cblas_elem(Order, B, ldb, k, j);
          else {
            pb = cblas_elem(Order, B, ldb, j, k);
            if (TransB == CblasConjTrans) {
              b_conj[0] = pb[0];
              b_conj[1] = -pb[1];
              pb = b_conj;
            }
          }

          float t[2];
          cmul(pa, pb, t);
          acc[0] += t[0];
          acc[1] += t[1];
        }
        /* acc = alpha * sum */
        float scaled[2];
        cmul(al, acc, scaled);
        acc[0] = scaled[0];
        acc[1] = scaled[1];
      }

      /* c = beta * c + acc */
      if (be[0] == 0.0f && be[1] == 0.0f) {
        c[0] = acc[0];
        c[1] = acc[1];
      } else {
        float bc[2];
        cmul(be, c, bc);
        c[0] = bc[0] + acc[0];
        c[1] = bc[1] + acc[1];
      }
      (void)zero;
    }
  }
}
