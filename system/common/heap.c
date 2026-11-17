#include "system/common/heap.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ALIGN8(x) (((x)+7)& ~((size_t)7)) //nice thing that basically just always rounds up to the next 8bit num
#define MIN_SPLIT_SIZE 16

#define HEAP_START ((void*)0x00200000)
#define HEAP_SIZE ((size_t)0x40000000) //1 gb (maybe 2 later but idk we only have 4 but most of that 4 is split between stack and heap so ig its fine)
//"only 4 gb ram"
//its a lot more than the 1 meg 16bit can use

typedef struct heap_block_header{
    alignas(8) size_t size; 
    alignas(8) bool isFree;
    alignas(8) struct heap_block_header* next;
    alignas(8) struct heap_block_header* prev;
} heap_block_header_t;

static struct heap_block_header* heap_head = NULL;

void heap_init(){
    if (!HEAP_START || HEAP_SIZE<= sizeof(heap_block_header_t)){
        return;
    }

    heap_head = (heap_block_header_t * )HEAP_START;
    heap_head->size = HEAP_SIZE-sizeof(heap_block_header_t);
    heap_head->isFree = true;
    heap_head->next = NULL;
    heap_head->prev = NULL;

}

void * malloc(size_t size){
    if (size==0||!heap_head){return NULL;}

    size_t alignedSize = ALIGN8(size);
    heap_block_header_t* current = heap_head;

    while(current!=NULL){
        if (current->isFree && current->size>=alignedSize){
            //both free and has avalabsel space:

            if (current->size>=alignedSize+sizeof(heap_block_header_t)+MIN_SPLIT_SIZE){
                //if enough to give space AND have a header
                heap_block_header_t* split = (heap_block_header_t*)((uint8_t*)(current+1)+alignedSize); //split tje block of mem if theres enough size for the desired bkicj and the ehader and stfuf
                split->size=current->size-alignedSize-sizeof(heap_block_header_t);
                split->isFree=true;
                split->next=current->next;
                split->prev=current;

                if (current->next!=NULL){
                    current->next->prev=split;
                }
                current->next=split;
                current->size=alignedSize;

            }

            current->isFree = false;
            return (void*)(current+1);

        }

        current = current->next;
        
    }
    return NULL; // oom
}

void free(void *pointer){
    if (!pointer){
        return;
    }

    heap_block_header_t* current = (heap_block_header_t*)pointer-1;
    current->isFree=true;
    if(current->next!=NULL&&current->next->isFree){
        //if next block is emptly then conoslodate
        current->size+=sizeof(heap_block_header_t)+current->next->size;
        current->next=current->next->next;
        if (current->next!=NULL){
            current->next->prev=current; //i will lose my mind if i read ts later
        }
    }

    if (current->prev!=NULL&&current->prev->isFree){ //vscode messed me up ;w; i love just doing current.var and tabbinfg to auto make it the -> but vscode denied me and it erred
        //same as before but for backwards conoslodation
        current->prev->size+=sizeof(heap_block_header_t)+current->size;
        current->prev->next=current->next;
        if (current->next!=NULL){
            current->next->prev=current->prev;
        }
    }
}

void *calloc(size_t num, size_t size){
    size_t total = size*num;
    uint8_t *ptr = (uint8_t*)malloc(total);
    if(!ptr){return NULL;}
    for (size_t i =0; i<total;i++){
        ptr[i]=0;
    }
    return (void*)ptr;
}

void* realloc(void* oldptr, size_t newsize){
    if (!oldptr){return malloc(newsize);}
    if (newsize==0){
        free(oldptr);
        return NULL;
    }
    heap_block_header_t* current = (heap_block_header_t*)oldptr-1;
    if(current->size>=newsize){
        return oldptr;
    }

    uint8_t* newptr = (uint8_t*) malloc(newsize);
    if (!newptr){
        return NULL;
    }

    uint8_t * useableoldptr = (uint8_t*)oldptr;
    for (size_t i = 0; i<current->size;i++){
        newptr[i]=useableoldptr[i];
    }

    free(oldptr);

    return (void*)newptr;

}