/* Link-only stubs: symbols needed to link nr-softmodem that are NOT
 * provided by real RISC-V shared libs (real libsctp/libz/libopenblas/
 * liblapacke replace the former library stubs). These symbols are baked
 * directly into the executable via an object file appended to the link
 * command, so they do not depend on any stub .so.
 *
 * - asn1c BER/DER runtime: RRC uses UPER, not BER/DER; these are pulled
 *   in only by generic ANY_t/constr_SET_OF support objects, not the RRC
 *   codec path. Stubs report failure so callers take their error path.
 * - OPENSSL_assert: no-op (matches the riscv64-stubs macro override).
 * - __builtin_cpu_init / __builtin_cpu_supports: x86-only GCC builtins;
 *   reporting "no x86 feature" is the CORRECT behavior on RISC-V (OAI
 *   then selects portable code paths; SIMDE handles SIMD separately).
 * - s1ap_config: zero blob => mme_enabled==0 => EPC_MODE_ENABLED==false,
 *   exactly what --noS1 / rfsim wants. */
#include <stddef.h>

struct asn_TYPE_descriptor_s;

typedef struct asn_enc_rval_stub_s {
    long                            encoded;       /* ssize_t */
    struct asn_TYPE_descriptor_s   *failed_type;
    void                           *structure_ptr;
} asn_enc_rval_stub_t;

typedef struct asn_dec_rval_stub_s {
    int  code;        /* RC_OK=0, RC_WMORE=1, RC_FAIL=2 */
    int  _pad;
    long consumed;    /* size_t */
} asn_dec_rval_stub_t;

asn_enc_rval_stub_t der_encode(void) {
    asn_enc_rval_stub_t r; r.encoded = -1; r.failed_type = 0; r.structure_ptr = 0; return r;
}
asn_dec_rval_stub_t ber_decode(void) {
    asn_dec_rval_stub_t r; r.code = 2; r._pad = 0; r.consumed = 0; return r;
}
asn_dec_rval_stub_t ber_check_tags(void) {
    asn_dec_rval_stub_t r; r.code = 2; r._pad = 0; r.consumed = 0; return r;
}
long der_write_tags(void) { return -1; }
int  asn_imax2INTEGER(void) { return -1; }

void OPENSSL_assert(int e) { (void)e; }

void __builtin_cpu_init(void) { /* no-op on RV64 */ }
int  __builtin_cpu_supports(const char *feature) { (void)feature; return 0; }

char s1ap_config[16384] = {0};
