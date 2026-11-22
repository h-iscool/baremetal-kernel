#include "system/common/display/desktop_screen.hpp"
#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"




DesktopScreen::DesktopScreen(Canvas * screen)
{
    mainScreen=screen;
}

DesktopScreen::~DesktopScreen()
{
}

void DesktopScreen::addObject(screenObject* object){
    if (objectCount>=MAX_UI_OBJECTS){return;}
    objects[objectCount] = object;
    objectCount++;
}

//uugh its gonna be a pain to make a remove or delete obj fn
//i could make it just kill the last but that would make it a pain to code later
//i could make it be kill # idx BUT then i need to be able to know what one is free or not so this is an issue for later


void DesktopScreen::render(){
    
    mainScreen->fillRect(0,0,mainScreen->width,mainScreen->height,0xFFFFFFFF); //ARGB

    for (int i = 0; i<objectCount; i++){
        
        screenObject* obj = objects[i];

        if (obj->visible){
            mainScreen->blit(*(obj->surface),obj->x,obj->y,obj->opacity);
        }

    }

    

}