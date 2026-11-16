#include <stdint.h>
#ifndef MACHINE_H
#define MACHINE_H

// void time_init(); // CURRENTLY CALLED IN ASM SO NO NEED TO EXPORT

// void timer_tick_call();

#ifdef __cplusplus
extern "C"{
#endif




void sleep(uint32_t ms);



#ifdef __cplusplus
}
#endif

#endif