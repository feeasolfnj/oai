#ifndef OPENSSL_MACROS_H
#define OPENSSL_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "openssl/opensslv.h"

/* Basic macros */
#define OPENSSL_NO_DEPRECATED_3_0

#define OPENSSL_assert(e) ((void)0)

#ifndef OPENSSL_UNUSED
#define OPENSSL_UNUSED(x) (void)(x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* OPENSSL_MACROS_H */
