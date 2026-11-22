#ifndef SCREEN_OBJ_HPP
#define SCREEN_OBJ_HPP

#include "system/common/display/canvas.hpp"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus



class screenObject
{
private:
    
public:

    Canvas * surface = nullptr;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int rotation = 0;
    uint8_t opacity = 255;
    int zIdx = 0;
    bool visible = true;

    

    screenObject(int width, int height);
    screenObject(int x, int y, int width, int height);
    ~screenObject();
    void setPosition(int newX, int newY);
    void translate(int deltaX, int deltaY);
    // void setRotation(int deg); //me when it hurts so i give up
    // void rotate(int deg);
    void setOpacity(uint8_t alpha);
    void changeOpacity(uint8_t alpha);
    bool contains(int px,int py);

};

#endif

#endif