#ifndef OPENSSL_ERR_H
#define OPENSSL_ERR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Error stubs - no-op */
#define ERR_put_error(lib, reason, file, line) ((void)0)
#define ERR_get_error() 0
#define ERR_clear_error() ((void)0)

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_ERR_H */
