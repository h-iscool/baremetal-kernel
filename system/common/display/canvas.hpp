#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <stdint.h>
#include "system/common/display/rect.hpp"

#ifdef __cplusplus

class Canvas
{
private:
    /* data */
public:

    uint32_t * buffer;
    int width;
    int height;
    int pitchpx;

    Canvas(uint32_t* buff3r, int w1dth, int h3ight, int p1tchInPx);
    ~Canvas();
    void putPixel(int x, int y, uint32_t color);
    void fillRect(int x, int y, int w, int h, uint32_t color);
    void blit(const Canvas& src, int destX, int destY, uint8_t opacity = 255);
    void drawBitmap(const uint32_t *  ARGBpixelBuffer , int destX, int destY, int w, int h);
    void fillCircle(int centerX, int centerY, int r, uint32_t color);
    void fillRect(int x, int y, int w, int h, uint32_t color, int r);
    void drawBitmap(const unsigned char *  ARGBpixelBuffer , int destX, int destY, int w, int h);

    void fill(const rect& area, uint32_t color);
    
    void copyFrom(const Canvas& src, const rect& area);

    void blit(const Canvas& src, int destX, int destY, uint8_t opacity, const rect& clip);

};

#endif

#endif