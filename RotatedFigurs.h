#ifndef ROTATEDFIGURS_H_INCLUDED
#define ROTATEDFIGURS_H_INCLUDED

void RLine(int x1, int y1, int x2, int y2, char orient, int posx, int posy){
    int tx1, ty1, tx2, ty2;
    posx*=50;
    posy*=50;
    posy+=50;
    if(orient=='u'){
        tx1= x1;
        ty1= y1;
        tx2= x2;
        ty2= y2;
    }
    if(orient=='r'){
        tx1= 50-y1;
        ty1= x1;
        tx2= 50-y2;
        ty2= x2;
    }
    if(orient=='d'){
        tx1= 50-x1;
        ty1= 50-y1;
        tx2= 50-x2;
        ty2= 50-y2;
    }
    if(orient=='l'){
        tx1= y1;
        ty1= 50-x1;
        tx2= y2;
        ty2= 50-x2;
    }
    line(tx1+posx,ty1+posy,tx2+posx,ty2+posy);
}

void RBar(int x1, int y1, int x2, int y2, char orient, int posx, int posy){
    int tx1, ty1, tx2, ty2;
    posx*=50;
    posy*=50;
    posy+=50;
    if(orient=='u'){
        tx1= x1;
        ty1= y1;
        tx2= x2;
        ty2= y2;
    }
        if(orient=='r'){
        tx1= 50-y1;
        ty1= x1;
        tx2= 50-y2;
        ty2= x2;
    }
        if(orient=='d'){
        tx1= 50-x1;
        ty1= 50-y1;
        tx2= 50-x2;
        ty2= 50-y2;
    }
    if(orient=='l'){
        tx1= y1;
        ty1= 50-x1;
        tx2= y2;
        ty2= 50-x2;
    }
    bar(tx1+posx,ty1+posy, tx2+posx,ty2+posy);
}
void RPolygon(int a[], int n, char orient, int posx, int posy){
    int tx1, ty1, tx2, ty2;
    posx*=50;
    posy*=50;
    posy+=50;
    int poly[n];
    if(orient=='u'){
        for(int i=0; i<n; i+=2){
            poly[i]= a[i] +posx;
            poly[i+1]= a[i+1] +posy;
        }
    }
    if(orient=='l'){
        for(int i=0; i<n; i+=2){
            poly[i]=a[i+1] +posx;
            poly[i+1]=a[i] +posy;
        }
    }
    if(orient=='r'){
        for(int i=0; i<n; i+=2){
            poly[i]= 49- a[i+1] +posx;
            poly[i+1]= 49- a[i] +posy;
        }
    }
    if(orient=='d'){
        for(int i=0; i<n; i+=2){
            poly[i]= 49- a[i] +posx;
            poly[i+1]= 49- a[i+1] +posy;
        }
    }
    fillpoly(n/2, poly);
}

void cegla(int x, int y){
    x*=50;
    y*=50;
    y+=50;
   setfillstyle(1, 6);
    bar(x+0,y+0, x+12,y+12);
    bar(x+13,y+0, x+37,y+12);
    bar(x+38,y+0, x+50,y+12);
    bar(x+0,y+13, x+24,y+25);
    bar(x+25,y+13, x+49,y+25);
    bar(x+0,y+26, x+12,y+38);
    bar(x+13,y+26, x+37,y+38);
    bar(x+38,y+26, x+50,y+38);
    bar(x+0,y+39, x+24,y+50);
    bar(x+25,y+39, x+49,y+50);
}
void blok(int x, int y){
    x*=50;
    y*=50;
    y+=50;
    setfillstyle(1, 8);
    bar(x+0,y+0, x+50,y+50);
    setfillstyle(1, 7);
    bar(x+4,y+4, x+46,y+46);
    setfillstyle(1, 15);
    bar(x+15,y+15, x+35,y+35);
}

void hearts(int lifes, int x, int y, int col){
    setfillstyle(1, col);
    setcolor(col);
    x*=50;
    y*=50;
    if(lifes>0){
        int poly1[16]={2+x,10+y, 3+x,7+y, 6+x,7+y, 8+x,10+y, 11+x,7+y, 14+x,7+y, 15+x,10+y, 8+x,17+y};
        fillpoly(8, poly1);
    }
    if(lifes>1){
        int poly2[16]={18+x,10+y, 19+x,7+y, 22+x,7+y, 24+x,10+y, 27+x,7+y, 30+x,7+y, 31+x,10+y, 24+x,17+y};
        fillpoly(8, poly2);
    }
    if(lifes>2){
        int poly3[16]={34+x,10+y, 35+x,7+y, 38+x,7+y, 40+x,10+y, 43+x,7+y, 46+x,7+y, 47+x,10+y, 40+x,17+y};
        fillpoly(8, poly3);
    }
}


#endif // ROTATEDFIGURS_H_INCLUDED
