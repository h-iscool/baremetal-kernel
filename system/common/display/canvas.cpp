#include "system/common/display/canvas.hpp"
#include "system/common/string.h"
#include "system/common/display/rect.hpp"

#include <stdint.h>




//blending helpers
static inline uint32_t div255(uint32_t div){
    div+=128;
    return((div+(div>>8))>>8);
}

static inline uint32_t blendOver(uint32_t src, uint32_t alph, uint32_t dest){
    uint32_t diff = 255-alph;
    uint32_t redandblue = (src & 0x00ff00ff) * alph + (dest & 0x00ff00ff) * diff + 0x00800080;
    redandblue=(((redandblue+((redandblue>>8)&0x00ff00ff))>>8)&0x00ff00ff); //im never touching rendering again after ts ;w;
    //legit what in the hell is this fake division bs
    uint32_t gren = ((src>>8)&0xff)*alph+((dest>>8)&0xff)*diff+128;
    gren = (((gren+(gren>>8))>>8)&0xff);

    return 0xFF000000 | redandblue | (gren<<8); //ikik we had a beautiful blender before but it messed stuff up sometim,es
}

















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

    // int idx = x+(y*pitchpx);
    // if (alpha==255){
    //     buffer[idx]=color;
    //     return;
    // }

    // uint32_t dest = buffer[idx];
    // uint32_t opac = 255-alpha;

    // uint32_t red = (((color>>16)&0xFF)*alpha+((dest>>16)&0xff)*opac+128)>>8;
    // uint32_t green = (((color>>8)&0xFF)*alpha+((dest>>8)&0xff)*opac+128)>>8;
    // uint32_t blue = (((color)&0xFF)*alpha+((dest)&0xff)*opac+128)>>8;

    // buffer[idx] = 0xFF000000 | (red<<16) | (green<<8) | (blue);

    uint32_t*pix = &buffer[x+(y*pitchpx)];
    *pix = (alpha==255)? color:blendOver(color,alpha,*pix); //w helper func

}


void Canvas::fill(const rect& area, uint32_t color){
    rect r = area.intersect(rect{0,0,width,height});
    for (int ro = 0; ro<r.h;ro++ ){
        //ro not row, obiously
        uint32_t* dest = buffer+(r.y+ro)*pitchpx+r.x;
        for (int i = 0; i<r.w;i++){
            dest[i]=color; //#itworks#trustmebro#DONOTFUCKINGTOUCHTHISPLZ
        }
    }
}

void Canvas::copyFrom(const Canvas& src, const rect& area){
    rect r = area.intersect(rect{0,0,width,height}).intersect(rect{0,0,src.width,src.height});
    //uk im getting to tired of this to use bs names ;w;
    //i cant read the code very well anymore ;w;w;w;w;w;
    for (int row = 0; row<r.h;row++){
        memcpy32_fast(buffer+(r.y+row)*pitchpx+r.x,src.buffer+(r.y+row)*src.pitchpx+r.x,r.w); //lets hope c+= never changes its order of operations
    }
}

//basic shapes:

void Canvas::fillRect(int x, int y, int w, int h, uint32_t color){

    // //ignore was asked for... ima make my own vars lmao
    // int x0 = x<0 ? /*if is less than 0 then 0*/ 0 /* else */ : x;
    // int y0 = y<0 ? /*if is less than 0 then 0*/ 0 /* else */ : y; 
    // int x1 = (x+w)>width /*if greater than cvs width*/? width /*set to cvs widt else set to normal wid*/:(x+w);
    // int y1 = (y+h)>height /*s,ae thjiong but y*/ ? height :(y+h);

    // for (int py = y0; py<y1;py++){
    //     for (int px = x0; px<x1; px++){
    //         putPixel(px,py,color);
    //     }
    // }

    rect r = rect{x,y,w,h}.intersect(rect{0,0,width,height});
    if (r.empty()){return;}

    uint32_t alph = color >>24;
    if (alph == 0){return;}
    //wait who tf calls alpha alph... hey alph sybau u wern't invited to this code
    if (alph==255){
        fill(r,color); return;
    }

    for (int py = 0; py<r.h;py++){
        uint32_t *dest=buffer+(r.y+py)*pitchpx+r.x;
        for (int px = 0; px<r.w;px++){
            dest[px]=blendOver(color,alph,dest[px]);
        }
    }

}




//these get fixed later

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



void Canvas::blit(const Canvas& src, int destX, int destY, uint8_t opacity){
    blit(src,destX,destY,opacity,rect{0,0,width,height});
}

void Canvas::blit(const Canvas& src, int destX, int destY, uint8_t opacity, const rect& clip){
    if (opacity == 0 ){
        return;
    }

    rect dest = rect{destX,destY,src.width,src.height}.intersect(clip).intersect(rect{0,0,width,height});

    if (dest.empty()){
        return;
    }

    int srcX0 = dest.x - destX;
    int srcY0 = dest.y - destY;

    for (int row=0;row<dest.h;row++){
        const uint32_t* s = src.buffer + (srcY0+row)*src.pitchpx+srcX0;
        uint32_t* d = buffer+(dest.y+row)*pitchpx+dest.x;

        if (opacity==255){
            for (int i = 0; i<dest.w;i++){
                uint32_t sclr = s[i];
                uint32_t a = sclr>>24;
                if (a == 255){
                    d[i]=sclr;
                }else if(a!=0){
                    d[i]=blendOver(sclr,a,d[i]);
                }
            }
        }else{
            for (int i = 0;i<dest.w;i++){
                uint32_t srclr = s[i];
                uint32_t a = div255((srclr>>24)*opacity);
                if(a!=0){
                    d[i]=blendOver(srclr,a,d[i]);
                }
            }
        }
    }
}


// void Canvas::blit(const Canvas& src, int destX, int destY, uint8_t opacity){ //bit-block transfur or some shit
    
//     if (opacity==0){return;}

//     for (int srcY = 0; srcY<src.height; srcY++){
//         int dy = destY+srcY;
//         if (dy<0||dy>=height){continue;}

//         for (int sx=0; sx<src.width; sx++){
//             int dx = destX+sx;
//             if (dx<0||dx>=width){continue;}

//             uint32_t srcpix = src.buffer[sx+(srcY*src.pitchpx)];

//             if (opacity == 255){
//                 putPixel(dx,dy,srcpix);
//             }else{
//                 uint32_t pxAlpha = (srcpix>>24)&0xff;
//                 uint32_t finalpha = (pxAlpha*opacity)>>8;
//                 uint32_t finalColor = (finalpha<<24)| (srcpix&0x00FFFFFF); //bitmasks are the goat
//                 putPixel(dx,dy,finalColor);
//             }
            
            

//         }
//     }


// }


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

