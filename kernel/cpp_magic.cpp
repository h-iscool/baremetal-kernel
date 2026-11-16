#include <stddef.h>
#include "../drivers/heap.h"
//cpp runtime bs:
extern "C"{
int atexit(void (*func)(void)){ // idgaf about memory cleanup rn cauwse we r shutting down the computr
        return 0;
}

void* __dso_handle = 0; // both of these just make compiler shut up lol
}

void * operator new(size_t size){
        return malloc(size);
}

void * operator new[](size_t size){
        return malloc(size);
}

void operator delete(void * ptr){
        free(ptr);
}

void operator delete[](void * ptr){
        free(ptr);
}

void operator delete(void * ptr, size_t size) noexcept{// promis to never throw an exeption
        (void)size;
        free(ptr);
}

void operator delete[](void * ptr, size_t size) noexcept{ 
        (void)size;
        free(ptr);
}