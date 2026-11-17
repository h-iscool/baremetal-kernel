#ifndef HEAP_H
#define HEAP_H
#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif
void heap_init();
void * malloc(size_t size);
void free(void *pointer);
void *calloc(size_t num, size_t size);
void* realloc(void* oldptr, size_t newsize);
#ifdef __cplusplus
}
#endif

#endif