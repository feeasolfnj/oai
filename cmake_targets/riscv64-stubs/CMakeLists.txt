# OpenSSL + CBLAS stubs library for RISC-V cross-compilation.
# cblas_stubs.c provides a naive cblas_cgemm (the only CBLAS routine OAI uses,
# from openair1/PHY/LTE_UE_TRANSPORT/linear_preprocessing_rec.c); cblas.h lives
# in include/ and is visible globally via the toolchain's -isystem.
add_library(ssl_stubs STATIC openssl_stubs.c cblas_stubs.c)
target_include_directories(ssl_stubs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
