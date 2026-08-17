/* Local override for gnu/stubs.h
 * When cross-compiling to RISC-V, CMake's pkg_check_modules adds
 * -I/usr/include/x86_64-linux-gnu for host libraries (blas, lapacke).
 * The x86 gnu/stubs.h then tries to include gnu/stubs-32.h which may not exist.
 * 
 * This local version provides minimal stubs definitions to allow compilation.
 */

/* Minimal stubs definitions */
#ifndef _GNU_STUBS_32_H
#define _GNU_STUBS_32_H
/* All function stubs are defined as unavailable */
#endif

#ifndef _GNU_STUBS_64_H
#define _GNU_STUBS_64_H
#endif

#ifndef _GNU_STUBS_X32_H
#define _GNU_STUBS_X32_H
#endif
