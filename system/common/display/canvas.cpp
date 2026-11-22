#include "system/common/display/canvas.hpp"

#include <stdint.h>



Canvas::Canvas(uint32_t* buff3r, int w1dth, int h3ight, int p1tchInPx){
    this->buffer = buff3r;
    this->width=w1dth;
    this->height=h3ight;
    this->pitchpx=p1tchInPx;


}




Canvas::~Canvas()
{
}





void Canvas::putPixel(int x, int y, uint32_t color){
    if (x<0||x>=width||y<0||y>=height){ return;}
    
    uint32_t alpha = (color>>24)&0xFF; //because the alpha is the first in ARGB format 0xAARRGGBB
    if (alpha==0) {return;}

    int idx = x+(y*pitchpx);
    if (alpha==255){
        buffer[idx]=color;
        return;
    }

    uint32_t dest = buffer[idx];
    uint32_t opac = 255-alpha;

    uint32_t red = (((color>>16)&0xFF)*alpha+((dest>>16)&0xff)*opac+128)>>8;
    uint32_t green = (((color>>8)&0xFF)*alpha+((dest>>8)&0xff)*opac+128)>>8;
    uint32_t blue = (((color)&0xFF)*alpha+((dest)&0xff)*opac+128)>>8;

    buffer[idx] = 0xFF000000 | (red<<16) | (green<<8) | (blue);
}


//basic shapes:

void Canvas::fillRect(int x, int y, int w, int h, uint32_t color){

    //ignore was asked for... ima make my own vars lmao
    int x0 = x<0 ? /*if is less than 0 then 0*/ 0 /* else */ : x;
    int y0 = y<0 ? /*if is less than 0 then 0*/ 0 /* else */ : y; 
    int x1 = (x+w)>width /*if greater than cvs width*/? width /*set to cvs widt else set to normal wid*/:(x+w);
    int y1 = (y+h)>height /*s,ae thjiong but y*/ ? height :(y+h);

    for (int py = y0; py<y1;py++){
        for (int px = x0; px<x1; px++){
            putPixel(px,py,color);
        }
    }

}


void Canvas::fillCircle(int centerX, int centerY, int r, uint32_t color){
    int x0 = (centerX-r<0)? 0 : centerX-r;
    int y0 = (centerY-r<0)? 0 : centerY-r;
    //do i really need to explain how these work again?
    int y1 = (centerY+r>=height)? height-1 : centerY+r;
    int x1 = (centerX+r>=width)?width-1 : centerX+r;

    int radsqrd = r*r;

    for (int py = y0; py<=y1;py++){
        for (int px = x0; px<=x1; px++){

            int destX = px-centerX;
            int destY = py-centerY;

            if ((destX*destX)+(destY*destY)<=radsqrd){
                putPixel(px,py,color);
            }

        }
    }
}

// void Canvas::fillRect(int x, int y, int w, int h, uint32_t color, int r){
//     //the rounded overload

//     //do circs for the corners:
//     fillCircle(x+r,y+r,r,color);
//     fillCircle(x+w-r-1,y+r,r,color);
//     fillCircle(x+w-r-1,y+h-r-1,r,color);
//     fillCircle(x+r,y+h-r-1,r,color);

//     fillRect(x,y+r+1,w,h-(r*2)-2,color);

//     fillRect(x+r+1,y,w-(r*2)-2,r+1,color);
//     fillRect(x+r+1,y+h-r-1,w-(r*2)-2,r+1,color);


// }


void Canvas::fillRect(int x, int y, int w, int h, uint32_t color, int r){
    int x0 = x<0?0:x;
    int y0 = y<0?0:y;
    int x1=(x+w)>width?width:(x+w);
    int y1=(y+h)>height?height:(y+h);
    int r2d = r*r;

    int topLcirX = x+r; int topLcircY = y+r;
    int topRcircX = x+w-r-1; int topRcircY = y+r;
    int bottomLcircX = x+r; int bottomLcircY = y+h-r-1;
    int bottomRcircX = x+w-r-1; int bottomRcircY = y+h-r-1;

    for (int py = y0; py<y1; py++){
        for (int px = x0; px<x1;px++){
            int dx=0;
            int dy=0;

            if (px<topLcirX && py< topLcircY){
                dx = topLcirX-px; 
                dy = topLcircY-py;
            }else if (px>topRcircX && py < topRcircY)
            {
                dx= px-topRcircX;
                dy = topRcircY-py;
            }else if (px<bottomLcircX&&py>bottomLcircY)
            {
                dx=bottomLcircX-px;
                dy = py-bottomLcircY;
            }else if (px>bottomRcircX&&py>bottomRcircY)
            {
                dx=px-bottomRcircX;
                dy = py-bottomRcircY;
            }

            if ((dx*dx)+(dy*dy)<=r2d){
                putPixel(px,py,color);
            }
            
            
            
        }
    }

}


void Canvas::blit(const Canvas& src, int destX, int destY, uint8_t opacity){ //bit-block transfur or some shit
    
    if (opacity==0){return;}

    for (int srcY = 0; srcY<src.height; srcY++){
        int dy = destY+srcY;
        if (dy<0||dy>=height){continue;}

        for (int sx=0; sx<src.width; sx++){
            int dx = destX+sx;
            if (dx<0||dx>=width){continue;}

            uint32_t srcpix = src.buffer[sx+(srcY*src.pitchpx)];

            if (opacity == 255){
                putPixel(dx,dy,srcpix);
            }else{
                uint32_t pxAlpha = (srcpix>>24)&0xff;
                uint32_t finalpha = (pxAlpha*opacity)>>8;
                uint32_t finalColor = (finalpha<<24)| (srcpix&0x00FFFFFF); //bitmasks are the goat
                putPixel(dx,dy,finalColor);
            }
            
            

        }
    }


}


void Canvas::drawBitmap(const uint32_t *  ARGBpixelBuffer , int destX, int destY, int w, int h){
    for (int srcY = 0; srcY<h;srcY++){
        int dy = destY+srcY;
        if (dy<0||dy>=height){continue;}
        for (int srcX = 0; srcX<w;srcX++){
            int dx = destX+srcX;
            if (dx<0 || dx>=width){continue;}
            putPixel(dx,dy,ARGBpixelBuffer[srcX+(srcY*w)]);
        }
    }
}

void Canvas::drawBitmap(const unsigned char *  ARGBpixelBuffer , int destX, int destY, int w, int h){
    for (int srcY = 0; srcY<h;srcY++){
        int dy = destY+srcY;
        if (dy<0||dy>=height){continue;}
        for (int srcX = 0; srcX<w;srcX++){
            int dx = destX+srcX;
            if (dx<0 || dx>=width){continue;}

            int byteidx = (srcX+(srcY*w))*4;

            uint32_t r = ARGBpixelBuffer[byteidx];
            uint32_t g = ARGBpixelBuffer[byteidx+1];
            uint32_t b = ARGBpixelBuffer[byteidx+2];
            uint32_t a = ARGBpixelBuffer[byteidx+3];

            uint32_t arbg = (a<<24) | (r<<16) | (g<<8) | b; //needs to be converted because of cpu endianness bs

            putPixel(dx,dy,arbg);
        }
    }
}

