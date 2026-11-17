#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

uint32_t adler32(const void *buf,size_t buflength);

#ifdef __cplusplus
}
#endif

#endif
