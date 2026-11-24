#ifndef DESKTOP_SCREEN_HPP
#define DESKTOP_SCREEN_HPP

#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"
#include "system/common/display/rect.hpp"
#include <stdbool.h>
 
#define MAX_UI_OBJECTS 128 //ikik 128 windows is a lot and ik ik its entirely possible to have issues cause u want more than 128 windows / desktop ui elements however ima do ts before dealing with making dynamic lists and stuff

#ifdef __cplusplus

class DesktopScreen
{
private:
    DamageList damage;
    bool fullRedraw=true;
public:

    Canvas * mainScreen; // back buffer
    Canvas * frontScreen; //real fbuffer
    screenObject* objects[MAX_UI_OBJECTS];
    int objectCount = 0;
    uint32_t backgroundColor = 0xffffffff;

    DesktopScreen(Canvas * backbuffer,Canvas*frpmtbuffer=nullptr);
    ~DesktopScreen();

    void addObject(screenObject* object);

    void invalidateAll();
    void invalidateRect(const rect& r);

    bool render();
};

#endif

#endif