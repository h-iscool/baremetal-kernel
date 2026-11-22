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
    x = newX;
    y = newY;
}

void screenObject::translate(int deltaX, int deltaY){
    x+=deltaX;
    // if (x<0){x=0;}
    // if(x>w){x=w;} DO NIOT RE ADD, MB IM A DUMBASS AND AM RESTRICTING MOVEMENT TO MOVEMENT WITHIN THE OBJECT
    y+=deltaY;
    // if (y<0){y=0;}
    // if (y>h){y=h;}
}

void screenObject::setOpacity(uint8_t alpha){
    // if (alpha>255){
    //     opacity=255;
    //     return;
    // }
    // if () im dumb ts is unsigned 8bit
    opacity = alpha;
}

void screenObject::changeOpacity(uint8_t alpha){
    uint16_t newopacity = (uint16_t)opacity + (uint16_t)alpha;
    if (newopacity>255){newopacity=255;}
    if (newopacity<0){newopacity=0;}
    opacity = (uint8_t)newopacity;
}

bool screenObject::contains(int px,int py){
    return (visible && //only if visible
    px>=x && px < (x+surface->width) &&
    py>=y && py < (y+surface->height) //basic aabb checkssssssss
    );
}