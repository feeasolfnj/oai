/* Minimal zlib stub for RISC-V cross-compilation linking. */
#include <stddef.h>
int deflate(void) { return 0; }
int deflateEnd(void) { return 0; }
int deflateInit_(void) { return 0; }
int deflateInit2_(void) { return 0; }
int inflate(void) { return 0; }
int inflateEnd(void) { return 0; }
int inflateInit_(void) { return 0; }
int inflateInit2_(void) { return 0; }
void *gzopen(void) { return NULL; }
int gzread(void) { return 0; }
int gzwrite(void) { return 0; }
int gzclose(void) { return 0; }
int compress(void) { return 0; }
int compressBound(void) { return 0; }
int uncompress(void) { return 0; }
long gzseek(void) { return -1; }
long gztell(void) { return -1; }
const char *gzerror(void) { return "stub"; }
unsigned long adler32(void) { return 0; }
unsigned long crc32(void) { return 0; }
