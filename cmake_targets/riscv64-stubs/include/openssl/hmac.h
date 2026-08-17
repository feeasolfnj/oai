#include <stddef.h>
#ifndef OPENSSL_HMAC_H
#define OPENSSL_HMAC_H

#include "openssl/evp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hmac_ctx_st {
    int dummy;
} HMAC_CTX;

HMAC_CTX* HMAC_CTX_new(void);
void HMAC_CTX_free(HMAC_CTX *ctx);
void HMAC_CTX_init(HMAC_CTX *ctx);
void HMAC_CTX_cleanup(HMAC_CTX *ctx);
int HMAC_Init_ex(HMAC_CTX *ctx, const void *key, int len, const EVP_MD *md, ENGINE *impl);
int HMAC_Update(HMAC_CTX *ctx, const unsigned char *data, size_t len);
int HMAC_Final(HMAC_CTX *ctx, unsigned char *md, unsigned int *md_len);

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_HMAC_H */
