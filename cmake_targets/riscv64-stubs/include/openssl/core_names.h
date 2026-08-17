#ifndef OPENSSL_CORE_NAMES_H
#define OPENSSL_CORE_NAMES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Core names for OpenSSL 3.0 algorithm fetching */
#define OSSL_MAC_NAME_HMAC "HMAC"
#define OSSL_MAC_NAME_CMAC "CMAC"
#define OSSL_DIGEST_NAME_SHA256 "SHA256"
#define OSSL_CIPHER_NAME_AES_128_CTR "AES-128-CTR"
#define OSSL_CIPHER_NAME_AES_128_ECB "AES-128-ECB"

/* OSSL_PARAM stubs */
typedef struct ossl_param_st {
    const char *key;
    unsigned int data_type;
    void *data;
    size_t data_size;
} OSSL_PARAM;

#define OSSL_PARAM_END { NULL, 0, NULL, 0 }
#define OSSL_PARAM_OCTET_STRING 1
#define OSSL_PARAM_UTF8_STRING 2
#define OSSL_PARAM_INTEGER 3
#define OSSL_PARAM_UNSIGNED_INTEGER 4

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_CORE_NAMES_H */
