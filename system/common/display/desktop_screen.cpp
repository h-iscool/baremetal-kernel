#include "system/common/display/desktop_screen.hpp"
#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"




DesktopScreen::DesktopScreen(Canvas * backbuffer,Canvas*frpmtbuffer)
{
    mainScreen=backbuffer;
    frontScreen=frpmtbuffer; // idc enough to fix it lol
}

DesktopScreen::~DesktopScreen()
{
}

void DesktopScreen::addObject(screenObject* object){
    if (objectCount>=MAX_UI_OBJECTS){return;}
    objects[objectCount] = object;
    objectCount++;
    object->stateDirty=true;
}

void DesktopScreen::invalidateAll(){
    fullRedraw=true;
}

void DesktopScreen::invalidateRect(const rect& r){
    damage.add(r.intersect(rect{0,0,mainScreen->width,mainScreen->height}));
}

//uugh its gonna be a pain to make a remove or delete obj fn
//i could make it just kill the last but that would make it a pain to code later
//i could make it be kill # idx BUT then i need to be able to know what one is free or not so this is an issue for later


bool DesktopScreen::render(){
    const rect screenRect{0,0,mainScreen->width,mainScreen->height};

    if (fullRedraw){
        damage.add(screenRect);
        fullRedraw=false;
    }



    for (int i =0;i<objectCount;i++){
        screenObject* obj = objects[i];

        rect current = obj->visible?rect{obj->x,obj->y,obj->surface->width,obj->surface->height}:rect{0,0,0,0};

        if (obj->stateDirty){
            damage.add(obj->lastRect.intersect(screenRect));
            damage.add(current.intersect(screenRect));
        }else if (!obj->dirtyContent.empty() && !current.empty()){
            rect abs{current.x+obj->dirtyContent.x,current.y+obj->dirtyContent.y,obj->dirtyContent.w,obj->dirtyContent.h};
            damage.add(abs.intersect(current).intersect(screenRect));
        }

        obj->lastRect=current;
        obj->stateDirty=false;
        obj->dirtyContent=rect{0,0,0,0};
    }

    if (damage.count==0){return false;}

    for (int r = 0; r<damage.count; r++){
        const rect clip = damage.rects[r];
        mainScreen->fill(clip,backgroundColor);

        for (int i=0;i<objectCount;i++){
            screenObject* obj = objects[i];
            if (!obj->visible||obj->opacity==0){continue;}
            rect orect{obj->x,obj->y,obj->surface->width,obj->surface->height};
            if (!orect.intersects(clip)){
                continue;
            }
            mainScreen->blit(*obj->surface,obj->x,obj->y,obj->opacity,clip);
        }
        if (frontScreen){
            frontScreen->copyFrom(*mainScreen,clip);
        }
    }

    damage.clear();
    return true;

}


// void DesktopScreen::render(){
    
//     mainScreen->fillRect(0,0,mainScreen->width,mainScreen->height,0xFFFFFFFF); //ARGB

//     for (int i = 0; i<objectCount; i++){
        
//         screenObject* obj = objects[i];

//         if (obj->visible){
//             mainScreen->blit(*(obj->surface),obj->x,obj->y,obj->opacity);
//         }

//     }

    

// }