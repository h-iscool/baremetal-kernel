#ifndef DESKTOP_SCREEN_HPP
#define DESKTOP_SCREEN_HPP

#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"
 
#define MAX_UI_OBJECTS 128 //ikik 128 windows is a lot and ik ik its entirely possible to have issues cause u want more than 128 windows / desktop ui elements however ima do ts before dealing with making dynamic lists and stuff

#ifdef __cplusplus

class DesktopScreen
{
private:
    
public:

    Canvas * mainScreen;
    screenObject* objects[MAX_UI_OBJECTS];
    int objectCount = 0;

    DesktopScreen(Canvas * screen);
    ~DesktopScreen();

    void addObject(screenObject* object);
    void render();
};

#endif

#endif