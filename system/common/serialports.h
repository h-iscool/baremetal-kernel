#ifndef SERIALPORTS_H
#define SERIALPORTS_H

#ifdef __cplusplus
extern "C"{
#endif

static inline unsigned char inb(unsigned short port){
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port)); // ATT SYNTAX: inb(read input byte)
    return result;
}

//__asm__ volatile ( "code" : outputs : inputs : clobbers );

static inline void outb(unsigned short port, unsigned char data){
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}



#ifdef __cplusplus
}
#endif

#endif