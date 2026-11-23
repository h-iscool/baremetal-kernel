#include "system/drivers/vbe_desktop.hpp"

#include "system/drivers/vbe_common.h"
#include "system/common/heap.h"
#include "system/common/string.h"

#include "system/common/display/desktop_screen.hpp"
#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"

#include <stdint.h>
#include <stddef.h>

static uint32_t * lfb_ptr = 0;
static uint32_t width = 0;
static uint32_t height = 0;
static uint32_t pitch = 0;
static uint32_t pitchpx = 0;
static size_t fb_bytes = 0;
static uint32_t * fbptr = 0;
static uint8_t loadedSuccessfully = 0;
static Canvas* screen = nullptr;


static uint8_t aspectRatioIdx = 0; // 0 = 21:9 1 = 16:9 2 = 4:3 all else is unsupported
typedef struct
{
    int width;
    int height;
} resolution;
int screenResolutionSetup(){
     // resolutions go like this:
     // bigger at top
     //for each commonly supported height (ex 1080)
     //go through checking 21:9 16:9 and 4:3
     //all other aspect ratios will just be unsupported because idk how to deal with this

     //also note this function will only ever be called by desktop init so i already know vbe support and scan res has run.

        const resolution resolutions[] = {
            //2160p
            {5120, 2160}, {3840, 2160}, {2880, 2160},
            //1440p
            {3440, 1440}, {2560, 1440}, {1920, 1440},
            //1080p
            {2560, 1080}, {1920, 1080}, {1440, 1080},
            //720p
            {1680, 720},  {1280, 720},  {960, 720},
            //480p
            {854, 480},   {640, 480}
        };

        const int numofchecks = sizeof(resolutions)/sizeof(resolutions[0]);

        for (int i = 0; i<numofchecks; i++){
            if (vbe_try_set_resolution(resolutions[i].width,resolutions[i].height)){
                lfb_ptr = vbe_get_fb_ptr();
                width = vbe_get_width();
                pitch = vbe_get_pitch();
                pitchpx = pitch/4;
                height = vbe_get_height();
                fb_bytes = width*height*sizeof(uint32_t);
                fbptr = (uint32_t*)calloc(1,fb_bytes);
                return 1;
            }
        }

     

    return 0;
}















void drawFrame(){
    memcpy32_fast(lfb_ptr,fbptr,fb_bytes/4);
}



#include "system/drivers/ps2_mouse.h"


void desktop_test_fn(){

    mouse_init();

    int mouseX = 0;
    int mouseY =0;
    if (!loadedSuccessfully || screen == nullptr) return;

    DesktopScreen compositor(screen);

    screenObject* window1 = new screenObject(100, 100, 400, 300);
    window1->surface->fillRect(0, 0, 400, 300, 0xFF3B82F6);
    screenObject* window2 = new screenObject(250, 200, 350, 250);
    window2->surface->fillRect(0, 0, 350, 250, 0xaFFC4899);
    window2->setOpacity(160);
    screenObject* window3 = new screenObject(0,0,500,600);
    window3->surface->fillRect(50,50,300,500,0x77ff00ff,12);
    window3->setOpacity(100);
    #include "system/common/display/assets/cursor.h"

    screenObject* cursorlayer = new screenObject(0,0,cursor_gimp_image.width,cursor_gimp_image.height);
    cursorlayer->surface->drawBitmap(cursor_gimp_image.pixel_data,0,0,cursor_gimp_image.width,cursor_gimp_image.height);

    compositor.addObject(window1);
    compositor.addObject(window2);
    compositor.addObject(window3);

    compositor.addObject(cursorlayer);

    while (1){


    

    mouse_packet_t packet = mouse_getPacket();
    
    int newx = mouseX + packet.deltaX;
    int newy = mouseY + packet.deltaY;
    
    int maxX = (int)width  - 1;
    int maxY = (int)height - 1;
    
    mouseX = newx < 0 ? 0 : (newx > maxX ? maxX : newx);
    mouseY = newy < 0 ? 0 : (newy > maxY ? maxY : newy);

    cursorlayer->setPosition(mouseX,mouseY);


    

    compositor.render();

    drawFrame();
    }
    
}






void desktop_init(){
    if (check_vbe_support()){
        vbe_scan_resolutions();
        if (screenResolutionSetup()){

            screen = new Canvas(fbptr, width, height, pitchpx);

            loadedSuccessfully=1;
            
        }
    }
}
