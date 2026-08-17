#include <stddef.h>
#ifndef OPENSSL_EVP_H
#define OPENSSL_EVP_H

#include "openssl/aes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;
typedef struct evp_cipher_st EVP_CIPHER;
typedef struct evp_md_st EVP_MD;
typedef struct evp_mac_st EVP_MAC;
typedef struct evp_mac_ctx_st EVP_MAC_CTX;
typedef struct ossl_lib_ctx_st OSSL_LIB_CTX;
typedef struct ossl_param_st OSSL_PARAM;
typedef struct engine_st ENGINE;

/* EVP_CIPHER_CTX functions */
EVP_CIPHER_CTX* EVP_CIPHER_CTX_new(void);
void EVP_CIPHER_CTX_free(EVP_CIPHER_CTX *ctx);
int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv);
int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl);
int EVP_EncryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);
int EVP_CIPHER_CTX_iv_length(const EVP_CIPHER_CTX *ctx);

/* Cipher algorithms */
const EVP_CIPHER* EVP_aes_128_ecb(void);
const EVP_CIPHER* EVP_aes_128_ctr(void);

/* Digest algorithms */
const EVP_MD* EVP_sha256(void);

/* MAC functions (OpenSSL 3.0 API) */
EVP_MAC* EVP_MAC_fetch(OSSL_LIB_CTX *libctx, const char *algorithm, const OSSL_PARAM *params);
void EVP_MAC_free(EVP_MAC *mac);
EVP_MAC_CTX* EVP_MAC_CTX_new(EVP_MAC *mac);
void EVP_MAC_CTX_free(EVP_MAC_CTX *ctx);
int EVP_MAC_init(EVP_MAC_CTX *ctx, const unsigned char *key, size_t keylen, const OSSL_PARAM *params);
int EVP_MAC_update(EVP_MAC_CTX *ctx, const unsigned char *data, size_t len);
int EVP_MAC_final(EVP_MAC_CTX *ctx, unsigned char *out, size_t *outlen, size_t outsize);

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_EVP_H */
