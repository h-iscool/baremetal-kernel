#ifndef CONSOLE_SCREEN_H
#define CONSOLE_SCREEN_H

#define DRIVER_VGA 1
#define DRIVER_VBE 2
#ifdef __cplusplus
extern "C"{
#endif

void printString(const char* string);
void printChar(char toPrint);
void clearScreen();
void initScreen(int screen);



#ifdef __cplusplus
}
#endif

#endif
