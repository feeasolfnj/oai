#include <stddef.h>
#include "openssl/aes.h"
#include "openssl/evp.h"
#include "openssl/hmac.h"
#include "openssl/cmac.h"
#include "openssl/err.h"

/* ============ AES stubs ============ */

int AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key) {
    (void)userKey; (void)bits; (void)key;
    return 0;
}

void AES_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key) {
    (void)in; (void)out; (void)key;
    /* Stub: in real implementation, this would encrypt */
}

/* ============ EVP stubs ============ */

struct evp_cipher_ctx_st {
    int dummy;
};

struct evp_cipher_st {
    int dummy;
};

struct evp_md_st {
    int dummy;
};

struct evp_mac_st {
    int dummy;
};

struct evp_mac_ctx_st {
    int dummy;
};

struct ossl_lib_ctx_st {
    int dummy;
};

EVP_CIPHER_CTX* EVP_CIPHER_CTX_new(void) { return NULL; }
void EVP_CIPHER_CTX_free(EVP_CIPHER_CTX *ctx) { (void)ctx; }
int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv) {
    (void)ctx; (void)type; (void)impl; (void)key; (void)iv; return 1;
}
int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl) {
    (void)ctx; (void)out; (void)outl; (void)in; (void)inl; return 1;
}
int EVP_EncryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl) {
    (void)ctx; (void)out; (void)outl; return 1;
}
int EVP_CIPHER_CTX_iv_length(const EVP_CIPHER_CTX *ctx) { (void)ctx; return 16; }

const EVP_CIPHER* EVP_aes_128_ecb(void) { static const EVP_CIPHER c = {0}; return &c; }
const EVP_CIPHER* EVP_aes_128_ctr(void) { static const EVP_CIPHER c = {0}; return &c; }
const EVP_MD* EVP_sha256(void) { static const EVP_MD m = {0}; return &m; }

/* ============ EVP_MAC stubs (OpenSSL 3.0 API) ============ */

EVP_MAC* EVP_MAC_fetch(OSSL_LIB_CTX *libctx, const char *algorithm, const OSSL_PARAM *params) {
    (void)libctx; (void)algorithm; (void)params; return NULL;
}
void EVP_MAC_free(EVP_MAC *mac) { (void)mac; }
EVP_MAC_CTX* EVP_MAC_CTX_new(EVP_MAC *mac) { (void)mac; return NULL; }
void EVP_MAC_CTX_free(EVP_MAC_CTX *ctx) { (void)ctx; }
int EVP_MAC_init(EVP_MAC_CTX *ctx, const unsigned char *key, size_t keylen, const OSSL_PARAM *params) {
    (void)ctx; (void)key; (void)keylen; (void)params; return 1;
}
int EVP_MAC_update(EVP_MAC_CTX *ctx, const unsigned char *data, size_t len) {
    (void)ctx; (void)data; (void)len; return 1;
}
int EVP_MAC_final(EVP_MAC_CTX *ctx, unsigned char *out, size_t *outlen, size_t outsize) {
    (void)ctx; (void)out; (void)outlen; (void)outsize; return 1;
}

/* ============ HMAC stubs ============ */

/* NOTE: struct hmac_ctx_st is now defined in the header (hmac.h) */

HMAC_CTX* HMAC_CTX_new(void) { return NULL; }
void HMAC_CTX_free(HMAC_CTX *ctx) { (void)ctx; }
void HMAC_CTX_init(HMAC_CTX *ctx) { (void)ctx; }
void HMAC_CTX_cleanup(HMAC_CTX *ctx) { (void)ctx; }
int HMAC_Init_ex(HMAC_CTX *ctx, const void *key, int len, const EVP_MD *md, ENGINE *impl) {
    (void)ctx; (void)key; (void)len; (void)md; (void)impl; return 1;
}
int HMAC_Update(HMAC_CTX *ctx, const unsigned char *data, size_t len) {
    (void)ctx; (void)data; (void)len; return 1;
}
int HMAC_Final(HMAC_CTX *ctx, unsigned char *md, unsigned int *md_len) {
    (void)ctx; (void)md; (void)md_len; return 1;
}

/* ============ CMAC stubs ============ */

CMAC_CTX *CMAC_CTX_new(void) { return NULL; }
void CMAC_CTX_free(CMAC_CTX *ctx) { (void)ctx; }
int CMAC_Init(CMAC_CTX *ctx, const void *key, size_t keylen, const EVP_CIPHER *cipher, void *engine) {
    (void)ctx; (void)key; (void)keylen; (void)cipher; (void)engine; return 1;
}
int CMAC_Update(CMAC_CTX *ctx, const unsigned char *data, size_t len) {
    (void)ctx; (void)data; (void)len; return 1;
}
int CMAC_Final(CMAC_CTX *ctx, unsigned char *out, size_t *outlen) {
    (void)ctx; (void)out; (void)outlen; return 1;
}

/* ============ Additional EVP cipher stubs ============ */

const EVP_CIPHER* EVP_aes_128_cbc(void) { static const EVP_CIPHER c = {0}; return &c; }
