#ifndef RECT_HPP
#define RECT_HPP

#include <stdbool.h>

struct rect
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    bool empty()const{
        return w <=0 || h<=0;
    }
    int right() const {
        return x + w;
    }
    int bottom() const{
        return y+h;
    }

    bool intersects(const rect& other) const{
        return !empty() && !other.empty()&&
            x<other.right() && other.x<right()&&
            y<other.bottom() && other.y<bottom() // i keep typint 0 instead of o in iotehr
        ; //w semioconolon on the next line cause funnis
        }

    rect intersect(const rect& other)const{ // NEED TO REMEMBER CONST TO NOT ACCIDNETALLY REDEF IT LATER
        int x0 = x>other.x?x:other.x; //get smolest x
        int y0 = y>other.y?y:other.y;
        int x1 = right()<other.right()?right():other.right(); //biggest
        int y1 = bottom()<other.bottom()?bottom():other.bottom();
        if (x1<=x0||y1<=y0){
            return rect{0,0,0,0};
        }
        return rect{x0,y0,x1-x0,y1-y0};
    } //intersect gives a huge rect htat has both

    rect unite/*d airlines or somethin (idk what to call this one, its the smolest that has both rects)*/(const rect& other)const{
        if (empty()){return other;}
        if (other.empty()){return *this;}
        int x0 = x<other.x?x:other.x;
        int y0 = y<other.y?y:other.y; //im loving the ? : operatiors (i can spel btw)
        int x1 = right() > other.right() ? right() : other.right();
        int y1 = bottom() > other.bottom() ? bottom() : other.bottom();
        return rect{x0,y0,x1-x0,y1-y0}; //have i made an xo joke yet, i forgor
        //idk idk gemotry daphhsh and such
    }

};



class DamageList{
    private:
    public:
    static const int max = 128;
    rect rects[max]; //ikik we LOVE being memory inefficent or something but idc to make dynamic lists still it seems so annyoing
    int count = 0;

    void clear()
    {
        count = 0;
    }; // useless multiline cause i love being trigger happy or something with the enter key
    
    void add(rect rectanglesssssss){
        if (rectanglesssssss.empty()){
            return;
            //i am slowly losing my mind with this proj arnt i....
            //u like kissing rectangles dont u
            // ^o.o^
        } //legit i write comments that sound like im high
        bool merged;
        do{
            merged=false;
            for (int i=0;i<count;i++){
                if (rects[i].intersects(rectanglesssssss)){
                    rectanglesssssss = rectanglesssssss.unite(rects[i]);
                    rects[i] = rects[count-1];
                    count--;
                    merged=true;
                    break;
                }
            }
        }while (merged); // do while is nice if u want to run something at least once
        if (count == max){
            for (int i = 0; i<count; i++){
                rectanglesssssss = rectanglesssssss.unite(rects[i]); //plz merge at some point please please please please please plea-
                count = 0;
            }
            
        }
        rects[count++] = rectanglesssssss;
    }
};

//this has got to be entertaining if anyone reads this code....

//burning_plant plz no look at htis bs 



#endif