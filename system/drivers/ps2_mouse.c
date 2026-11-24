#include "system/drivers/ps2_mouse.h"

#include "system/common/serialports.h"

#include <stdbool.h>


mouse_packet_t mousePacketExport;

static int mouseID = 0;
int mouseCycle = 0;
unsigned char mousePacket[4];
int packetsize = 0;


void mouse_waitUntilRW(unsigned char type){
    int timeout = 100000;
    switch (type)
    {
    case 0:
        
        while(timeout--){
            if ((inb(0x64)&1)==1){
                //0x64 is ps2
                return;
            }
        }

        break;
    case 1:

        while(timeout--){
            if ((inb(0x64)&2)==0){
                //0x64 is ps2
                return;
            }
        }

        break;
    default:
        break;
    }
}

void mouse_write(unsigned char cmd){
    mouse_waitUntilRW(1);
    outb(0x64,0xd4); //0xd4 == next byte sent is for mouse
    mouse_waitUntilRW(1);
    outb(0x60,cmd); //0x60 is the inp for data that will be forwarded
    mouse_waitUntilRW(0);
    inb(0x60); //ack byte
}




void mouse_init(){

    __asm__ volatile("cli");

    while (inb(0x64)&1){
        inb(0x60);
    }

    mouse_waitUntilRW(1);
    outb(0x64,0xa8); //enables aux mouse device

    mouse_waitUntilRW(1);
    outb(0x64,0x20); // read stat byte
    mouse_waitUntilRW(0);
    unsigned char status = inb(0x60);

    status |=2; // i just found out u can do that also ts enables the irq12 bit
    status &=~0x20; //enable mouse clock
    status &=~0x10;

    mouse_waitUntilRW(1);
    outb(0x64,0x60); // write status byte;
    mouse_waitUntilRW(1);
    outb(0x60,status);

    mouse_write(0xf6); // make mouse use defaults


    // enable scroll and mb4 and mb5
    mouse_write(0xf3);
    mouse_write(200);
    mouse_write(0xf3); // f3 changes sample rate
    mouse_write(100);
    mouse_write(0xf3);
    mouse_write(80);
    //200 100 80 = scroll

    mouse_write(0xf3);
    mouse_write(200);
    mouse_write(0xf3);
    mouse_write(200);
    mouse_write(0xf3);
    mouse_write(80);
    //200 200 80 = 5 buttons
    //microslop be vibecoding shit from the very beginning
    //these magic # pmo

    

    mouse_write(0xf2); //obtain device id
    mouse_waitUntilRW(0);
    mouseID = inb(0x60);

    mouse_write(0xf3);
    mouse_write(200); // reset sample rate to 200hz

    packetsize = (mouseID==3||mouseID==4)?4:3;
    mouseCycle=0;


    mouse_write(0xf4); //data reportiung enabled

    while (inb(0x64)&1){
        inb(0x60);
    }

    outb(0x21, inb(0x21) & ~(1u << 2));
    outb(0xa1, inb(0xa1) & ~(1u << 4));

    __asm__ volatile("sti");

}






void mouse_int_handler(){
    unsigned char status = inb(0x64);

    if ((status&0x01)&&(status&0x20)){
        unsigned char data = inb(0x60);

        if (mouseCycle==0 && !(data&0x08)){
            outb(0xa0, 0x20);
            outb(0x20,0x20);
            return;
        }

        mousePacket[mouseCycle++] = data;

        if (mouseCycle==packetsize){
            mouseCycle=0;

            int dx = mousePacket[1];
            int dy = mousePacket[2];
            if (mousePacket[0]&0x10){
                dx-=256;
            }
            if (mousePacket[0]&0x20){
                dy-=256;
            }
            dy=-dy;

            mousePacketExport.mbLeft = mousePacket[0] & 0x01;
            mousePacketExport.mbRight = mousePacket[0] & 0x02;
            mousePacketExport.mbMiddle = mousePacket[0] & 0x04;

            mousePacketExport.deltaX += dx;
            mousePacketExport.deltaY += dy;

            if (mouseID == 3){
                mousePacketExport.deltaScroll += (signed char) mousePacket[3];
            }else if (mouseID == 4){
                int dz = mousePacket[3]&0x0f;
                if (dz&0x08){
                    dz-=16;
                }
                mousePacketExport.deltaScroll += dz;
                mousePacketExport.mb4 = mousePacket[3] & 0x10;
                mousePacketExport.mb5 = mousePacket[3] & 0x20;
            }


        }

    }

    outb(0xa0, 0x20);
    outb(0x20,0x20);
    return;

}


mouse_packet_t mouse_getPacket(){

    __asm__ volatile ("cli");

    mouse_packet_t toret;

    toret.deltaX = mousePacketExport.deltaX;
    toret.deltaY = mousePacketExport.deltaY;
    toret.deltaScroll = mousePacketExport.deltaScroll;
    toret.mbLeft = mousePacketExport.mbLeft;
    toret.mbRight = mousePacketExport.mbRight;
    toret.mbMiddle = mousePacketExport.mbMiddle;
    toret.mb4 = mousePacketExport.mb4;
    toret.mb5 = mousePacketExport.mb5;


    mousePacketExport.deltaX = 0;
    mousePacketExport.deltaY = 0;
    mousePacketExport.deltaScroll = 0;

    // mousePacketExport.mbLeft = false;
    // mousePacketExport.mbRight = false;
    // mousePacketExport.mbMiddle = false;

    // mousePacketExport.mb4 = false;
    // mousePacketExport.mb5 = false;

    __asm__ volatile ("sti");

    return toret;
}