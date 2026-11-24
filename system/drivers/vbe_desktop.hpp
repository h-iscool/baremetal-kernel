#ifndef VBE_DESKTOP_H
#define VBE_DESKTOP_H
#ifdef __cplusplus 
//cpp ports

#include "system/common/display/canvas.hpp"
Canvas* desktop_back_canvas();
Canvas* desktop_front_canvas();


extern "C"{// C ports
#endif

void desktop_init();
// void desktop_test_fn(); //rip, this fn carried me for so long...

#ifdef __cplusplus
}
#endif
#endif