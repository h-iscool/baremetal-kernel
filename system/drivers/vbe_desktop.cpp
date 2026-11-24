#include "system/drivers/vbe_desktop.hpp"

#include "system/drivers/vbe_common.h"
#include "system/common/heap.h"
#include "system/common/string.h"

#include "system/common/display/desktop_screen.hpp"
#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"

#include "system/drivers/ps2_mouse.h"

#include <stdint.h>
#include <stddef.h>

static uint32_t * lfb_ptr = 0;
static uint32_t width = 0;
static uint32_t height = 0;
static uint32_t pitchpx = 0;
static uint32_t * backbuffer = 0;
static Canvas* backCanvas = nullptr;
static Canvas* frontCanvas = nullptr;

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
                pitchpx = vbe_get_pitch()/4;
                height = vbe_get_height();
                return 1;
            }
        }

     

    return 0;
}














Canvas* desktop_back_canvas(){
    return backCanvas;
}

Canvas* desktop_front_canvas(){
    return frontCanvas;
}




void desktop_init(){
    if (!check_vbe_support()){
        return;
    }
    vbe_scan_resolutions();
    if (!screenResolutionSetup()){
        return;
    }

    backbuffer = (uint32_t*)calloc(1,(size_t)width*height*sizeof(uint32_t));
    if (!backbuffer){
        return;
    }

    backCanvas = new Canvas(backbuffer,(int)width, (int)height,(int)width); //no pitch cause this is the buffer that getrs ported tio the actual screen mem
    frontCanvas = new Canvas(lfb_ptr, (int)width,(int)height,(int)pitchpx); // pitch was useless cause i always mult to get pitch px anywayts

    mouse_init();
    

}