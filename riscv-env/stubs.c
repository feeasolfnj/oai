/* Stub shared library for RISC-V cross-compilation.
 * Provides empty implementations of libsctp, libblas, and liblapacke
 * functions so the linker can resolve symbols. At runtime, calling
 * these will return errors/no-ops ¡ª sufficient for "just needs to start"
 * testing. For actual PHY layer processing, replace with real RISC-V
 * BLAS/LAPACK and SCTP libraries.
 *
 * Phase-1 link additions (2026-08-09): asn1c BER/DER runtime stubs,
 * OpenSSL assert, x86 __builtin_cpu_* shims, S1AP config blob.
 * These are needed to get the nr-softmodem ELF to link. RRC actually
 * uses UPER (uper_encode_to_buffer), not BER/DER, so the asn1c BER/DER
 * stubs are not on the RRC encode/decode hot path. */

#include <stddef.h>

/* ===== libsctp stubs ===== */
int  sctp_sendmsg(void) { return -1; }
int  sctp_recvmsg(void) { return -1; }
int  sctp_bindx(void)   { return -1; }
int  sctp_connectx(void){ return -1; }
int  sctp_getladdrs(void){ return 0; }
int  sctp_getpaddrs(void){ return 0; }
void sctp_freeladdrs(void){}
void sctp_freepaddrs(void){}
int  sctp_peeloff(void)  { return -1; }

/* ===== libblas stubs (CBLAS interface) ===== */
void cblas_cgemm(void)  {}
void cblas_zaxpy(void)  {}
void cblas_zcopy(void)  {}
void cblas_zgemv(void)  {}
void cblas_zscal(void)  {}

/* ===== liblapacke stubs ===== */
int  LAPACKE_cgetrf(void){ return 0; }
int  LAPACKE_cgetrs(void){ return 0; }
int  LAPACKE_cheev(void) { return 0; }

/* =====================================================================
 * Phase-1 link stubs: asn1c BER/DER runtime
 * ---------------------------------------------------------------------
 * der_encode / ber_decode / ber_check_tags return asn1c structs (24 / 16
 * / 16 bytes). On RV64 LP64D the 24-byte struct is returned via a hidden
 * sret pointer (a0); the 16-byte structs are returned in a0/a1. The struct
 * layouts below EXACTLY mirror asn_codecs.h so the compiler emits the
 * correct return ABI. Argument lists are stubbed as (void): the calling
 * convention places args in registers regardless, and the stubs ignore
 * them. All stubs report failure (encoded=-1 / code=RC_FAIL) so callers
 * take their error path rather than consuming uninitialized buffers.
 *
 * RRC messaging uses UPER (uper_encode_to_buffer / uper_decode), not
 * BER/DER; these symbols are pulled in only by the generic ANY_t /
 * constr_SET_OF / constr_SEQUENCE_OF support objects. They are not on
 * the RRC codec path for a --noS1 rfsim boot.
 * ===================================================================== */
struct asn_TYPE_descriptor_s;  /* forward decl, matches generated headers */

/* asn_enc_rval_t: { ssize_t encoded; ptr failed_type; ptr structure_ptr } = 24 B */
typedef struct asn_enc_rval_stub_s {
    long                            encoded;       /* ssize_t */
    struct asn_TYPE_descriptor_s   *failed_type;
    void                           *structure_ptr;
} asn_enc_rval_stub_t;

/* asn_dec_rval_t: { enum code (int); pad; size_t consumed } = 16 B */
typedef struct asn_dec_rval_stub_s {
    int  code;        /* RC_OK=0, RC_WMORE=1, RC_FAIL=2 */
    int  _pad;
    long consumed;    /* size_t */
} asn_dec_rval_stub_t;

/* der_encode -> asn_enc_rval_t (24 B, sret). encoded=-1 = failure. */
asn_enc_rval_stub_t der_encode(void) {
    asn_enc_rval_stub_t r;
    r.encoded = -1;
    r.failed_type = 0;
    r.structure_ptr = 0;
    return r;
}

/* ber_decode -> asn_dec_rval_t (16 B, a0/a1). code=RC_FAIL(2). */
asn_dec_rval_stub_t ber_decode(void) {
    asn_dec_rval_stub_t r;
    r.code = 2;        /* RC_FAIL */
    r._pad = 0;
    r.consumed = 0;
    return r;
}

/* ber_check_tags -> asn_dec_rval_t (16 B, a0/a1). code=RC_FAIL(2). */
asn_dec_rval_stub_t ber_check_tags(void) {
    asn_dec_rval_stub_t r;
    r.code = 2;        /* RC_FAIL */
    r._pad = 0;
    r.consumed = 0;
    return r;
}

/* der_write_tags -> ssize_t (scalar). -1 = failure. */
long der_write_tags(void) { return -1; }

/* asn_imax2INTEGER -> int. -1 = failure. (Called from x2ap_eNB_generate_messages.c
 * with an implicit declaration; asn1c runtime helper, not on UPER RRC path.) */
int asn_imax2INTEGER(void) { return -1; }

/* =====================================================================
 * Phase-1 link stubs: OpenSSL
 * ---------------------------------------------------------------------
 * cmake_targets/riscv64-stubs/include/openssl/macros.h defines
 * OPENSSL_assert(e) as ((void)0), but TUs that do not see that override
 * (e.g. openair3/SECU/aes_128_ctr.c) emit a real function call. Provide a
 * no-op function so the link resolves. Never aborts ¡ª safe for boot.
 * ===================================================================== */
void OPENSSL_assert(int e) { (void)e; }

/* =====================================================================
 * Phase-1 link shims: x86 GCC CPU-detection builtins
 * ---------------------------------------------------------------------
 * __builtin_cpu_init / __builtin_cpu_supports are x86-only GCC builtins.
 * RISC-V GCC does not provide them, so calls remain as undefined external
 * symbols. OAI's own code (common/utils/LOG/log.c, openair1/PHY/TOOLS/
 * cmult_sv.c) uses them to pick x86 fast paths. Reporting "no x86 feature
 * supported" makes OAI fall back to the portable C path ¡ª which is the
 * CORRECT behavior on RISC-V (SIMDE handles SIMD separately). These are
 * shims, not mere stubs.
 * ===================================================================== */
void __builtin_cpu_init(void) { /* no-op: nothing to probe on RV64 */ }

int __builtin_cpu_supports(const char *feature) {
    /* No x86 SIMD feature exists on RISC-V; always report unsupported so
     * callers select the portable code path. */
    (void)feature;
    return 0;
}

/* =====================================================================
 * Phase-1 link stubs: S1AP global config
 * ---------------------------------------------------------------------
 * s1ap_eNB.c was stubbed (original backed up as .c.orig) for the asn1c
 * interface mismatch, which dropped the definition of the global
 * `s1ap_config` (type s1ap_eNB_config_t, declared extern in s1ap_eNB.h).
 * Provide a zero-initialized blob. zero => mme_enabled == 0 =>
 * EPC_MODE_ENABLED == false, which is exactly what --noS1 / rfsim wants.
 * The blob is oversized (16 KiB) so any field access lands inside it;
 * stubs.c does not include s1ap_eNB.h, so there is no type conflict at
 * compile time, and the linker merges symbols by name.
 * ===================================================================== */
char s1ap_config[16384] = {0};
