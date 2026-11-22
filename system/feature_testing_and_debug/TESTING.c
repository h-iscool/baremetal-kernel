#include "system/drivers/console_screen.h"
#include "system/common/machine.h"
#include "system/drivers/vbe_desktop.hpp"

void LOAD_TESTING(){ //ikik its c not asm but C is so much more readable. also we should try to keep this file kinda clean... maybe

    printString("Loading VBE\n");

    // sleep(1000);

    
    desktop_init();

    desktop_test_fn();



    while (1){ //comment to continue, uncomment to hlt before loading the console
        __asm__ ("hlt");
    }

}