#include "system/common/display/desktop_runtime.hpp"

#include "system/drivers/vbe_desktop.hpp"
#include "system/drivers/ps2_mouse.h"

#include "system/common/display/desktop_screen.hpp"
#include "system/common/display/canvas.hpp"
#include "system/common/display/screen_obj.hpp"
#include "system/common/display/assets/cursor.h"



Canvas* back = nullptr;
Canvas * front = nullptr;
int width,height = 0;
int mouseX=0;
int mouseY=0;

DesktopScreen* compositor= nullptr;

screenObject* window1 = nullptr;
screenObject* window2 = nullptr;

screenObject* window3 = nullptr;

screenObject* cursorlayer = nullptr;


void desktopDisplayInit(){
    desktop_init();

    back = desktop_back_canvas();
    front = desktop_front_canvas();
    if (!front||!back){return;}

    width = back->width;
    height = back->height;

    compositor =new DesktopScreen(back,front);

    window1 = new screenObject(100, 100, 400, 300);
    window2 = new screenObject(250, 200, 350, 250);
    window3 = new screenObject(0, 0, 500, 600);

    cursorlayer =new screenObject(0, 0, cursor_gimp_image.width, cursor_gimp_image.height);


    
    window1->surface->fillRect(0, 0, 400, 300, 0xFF3B82F6);

    window2->surface->fillRect(0, 0, 350, 250, 0xAFFC4899);
    window2->setOpacity(160);
    
    window3->surface->fillRect(50, 50, 300, 500, 0x77ff00ff, 12);
    window3->setOpacity(100);


    cursorlayer->surface->drawBitmap(cursor_gimp_image.pixel_data, 0, 0,cursor_gimp_image.width, cursor_gimp_image.height);


    compositor->addObject(window1);
    compositor->addObject(window2);
    compositor->addObject(window3);
    compositor->addObject(cursorlayer);
    
}

void desktopDisplayTick(){
    if (!front||!back){return;}

    mouse_packet_t packet = mouse_getPacket();

    if (packet.deltaX!=0||packet.deltaY!=0){
        int newx=mouseX+packet.deltaX;
        int newy=mouseY+packet.deltaY;
        mouseX=newx<0?0:(newx>width-1?width-1:newx);
        mouseY=newy<0?0:(newy>height-1?height-1:newy);
        cursorlayer->setPosition(mouseX,mouseY);
    }

    if (!compositor->render()){
        __asm__ volatile("hlt");
    }

}