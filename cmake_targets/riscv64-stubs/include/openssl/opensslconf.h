#ifndef OPENSSL_CONF_H
#define OPENSSL_CONF_H

/* Minimal OpenSSL configuration for RISC-V cross-compilation */
#define OPENSSL_NO_SECURE_MEMORY
#define OPENSSL_NO_DYNAMIC
#define OPENSSL_NO_ENGINE
#define OPENSSL_NO_SSL3
#define OPENSSL_NO_TLS1
#define OPENSSL_NO_TLS1_1
#define OPENSSL_NO_TLS1_2

#endif /* OPENSSL_CONF_H */
