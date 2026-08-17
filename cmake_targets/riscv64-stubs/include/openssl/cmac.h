#ifndef OPENSSL_CMAC_H
#define OPENSSL_CMAC_H

#include "openssl/evp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* CMAC_CTX type definition */
typedef struct cmac_ctx_st {
    int dummy;
} CMAC_CTX;

/* CMAC function stubs */
CMAC_CTX *CMAC_CTX_new(void);
void CMAC_CTX_free(CMAC_CTX *ctx);
int CMAC_Init(CMAC_CTX *ctx, const void *key, size_t keylen, const EVP_CIPHER *cipher, void *engine);
int CMAC_Update(CMAC_CTX *ctx, const unsigned char *data, size_t len);
int CMAC_Final(CMAC_CTX *ctx, unsigned char *out, size_t *outlen);

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_CMAC_H */
