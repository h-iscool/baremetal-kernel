#ifndef PS_2_MOUSE_H
#define PS_2_MOUSE_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    bool mbLeft;
    bool mbRight;
    bool mbMiddle;
    bool mb4;
    bool mb5;
    int deltaX;
    int deltaY;
    int deltaScroll;
}mouse_packet_t;

void mouse_init();

mouse_packet_t mouse_getPacket();

#ifdef __cplusplus
}
#endif
#endif