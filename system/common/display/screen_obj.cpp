#include "system/common/display/screen_obj.hpp"
#include "system/common/display/canvas.hpp"
#include <stdint.h>
#include <stdbool.h>



screenObject::screenObject(int width, int height)
{
    w=width;
    h=height;
    surface=new Canvas(new uint32_t[w*h](),w,h,w);
}

screenObject::screenObject(int xPos, int yPos, int width, int height){
    x = xPos;
    y= yPos;
    w = width;
    h = height;
    surface=new Canvas(new uint32_t[w*h](),w,h,w);
}

screenObject::~screenObject()
{
    delete[] surface->buffer;
    delete surface;
}

void screenObject::setPosition(int newX, int newY){
    if (newX==x&&newY==y){return;}
    x = newX;
    y = newY;
    stateDirty=true;
}

void screenObject::translate(int deltaX, int deltaY){
    if (deltaX==0&deltaY==0){return;}
    x+=deltaX;
    // if (x<0){x=0;}
    // if(x>w){x=w;} DO NIOT RE ADD, MB IM A DUMBASS AND AM RESTRICTING MOVEMENT TO MOVEMENT WITHIN THE OBJECT
    y+=deltaY;
    // if (y<0){y=0;}
    // if (y>h){y=h;}
    stateDirty=true;
}

void screenObject::setOpacity(uint8_t alpha){
    if (alpha==opacity){return;}
    // if (alpha>255){
    //     opacity=255;
    //     return;
    // }
    // if () im dumb ts is unsigned 8bit
    opacity = alpha;
    stateDirty=true;
}

void screenObject::changeOpacity(uint8_t alpha){
    uint16_t newopacity = (uint16_t)opacity + (uint16_t)alpha;
    if (newopacity>255){newopacity=255;}
    if (newopacity<0){newopacity=0;}
    setOpacity((uint8_t)newopacity);
}

bool screenObject::contains(int px,int py){
    return (visible && //only if visible
    px>=x && px < (x+surface->width) &&
    py>=y && py < (y+surface->height) //basic aabb checkssssssss
    );
}

void screenObject::setVisible(bool v){
    if (v==visible){return;}
    visible = v;
    stateDirty=true;
}

void screenObject::invalidate(){
    dirtyContent=rect{0,0,surface->width,surface->height};
}

void screenObject::invalidate(const rect&  localchange){
    dirtyContent=dirtyContent.unite(localchange.intersect(rect{0,0,surface->width,surface->height}));
}